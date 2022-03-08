#include "ECS/Model.h"
#include "Core/System/Application.h"

#include "Core/Debug/Log.h"

void ECS::Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		MYTH_ERROR("ASSIMP::%s", importer.GetErrorString());
		return;
	}

	m_directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(path, scene->mRootNode, scene);
}

void ECS::Model::ProcessNode(const std::string& p_nodeName, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Ressources::RessourcesManager& rm = Core::System::Application::GetInstance().GetRessourcesManager();
		m_meshes.push_back(ProcessMesh((p_nodeName + "_mesh[" + std::to_string(i) + "]"), mesh, scene));
		rm.RegisterAndLoadRessource(m_meshes.back());
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode((p_nodeName + "_child[" + std::to_string(i) + "]"), node->mChildren[i], scene);
	}
}

void ProcessVertex(Vertex& vertex, aiMesh* mesh, unsigned int indice)
{
	vertex.position.m_x = mesh->mVertices[indice].x;
	vertex.position.m_y = mesh->mVertices[indice].y;
	vertex.position.m_z = mesh->mVertices[indice].z;

	vertex.normal.m_x = mesh->mNormals[indice].x;
	vertex.normal.m_y = mesh->mNormals[indice].y;
	vertex.normal.m_z = mesh->mNormals[indice].z;

	if (mesh->mTextureCoords[0])
	{
		vertex.uv.m_x = mesh->mTextureCoords[0][indice].x;
		vertex.uv.m_y = mesh->mTextureCoords[0][indice].y;
	}
	else
		vertex.uv = Vec2{ 0.f, 0.f };
}

Ressources::Mesh* ECS::Model::ProcessMesh(const std::string& p_meshName, aiMesh* mesh, const aiScene* scene)
{
	Ressources::Mesh* tmp_mesh = new Ressources::Mesh(p_meshName);
	std::vector<Ressources::Texture*> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		ProcessVertex(vertex, mesh, i);
		tmp_mesh->m_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			tmp_mesh->m_indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Ressources::Texture*> diffuseMaps;
		LoadMaterialTextures(p_meshName, material, aiTextureType_DIFFUSE, "texture_diffuse", diffuseMaps);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Ressources::Texture*> specularMaps;
		LoadMaterialTextures(p_meshName, material, aiTextureType_SPECULAR, "texture_specular", specularMaps);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	tmp_mesh->m_gpu = std::make_shared<GPUMesh>();
	tmp_mesh->m_gpu->VertexCount = tmp_mesh->m_vertices.size();
	tmp_mesh->m_gpu->offset = m_allVertices.size();
	m_allVertices.insert(m_allVertices.end(), tmp_mesh->m_vertices.begin(), tmp_mesh->m_vertices.end());
	return tmp_mesh;
}

void ECS::Model::LoadMaterialTextures(const std::string& p_meshName, aiMaterial* mat, aiTextureType type,
									  const std::string& typeName, std::vector<Ressources::Texture*>& textures)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_loadedTextures.size(); j++)
		{
			if (std::strcmp(m_loadedTextures[j]->m_path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Ressources::Texture* texture = new Ressources::Texture(p_meshName + "_" + typeName + "[" + std::to_string(i) + "]");
			texture->m_type = typeName;
			texture->m_path = m_directory + '/' + std::string(str.C_Str());
			Ressources::RessourcesManager& rm = Core::System::Application::GetInstance().GetRessourcesManager();
			textures.push_back(texture);
			m_loadedTextures.push_back(textures.back());
			rm.RegisterAndLoadRessource(m_loadedTextures.back());
		}
	}

	return;
}

void ECS::Model::Draw(Core::GameObject* p_gameObject)
{
	if (m_loadedTextures.size() != 0)
		glBindTexture(GL_TEXTURE_2D, m_loadedTextures[0]->m_gpu->data);
}
