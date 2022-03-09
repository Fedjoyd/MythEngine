#include "ECS/Model.h"
#include "Core/System/Application.h"

#include "Core/Debug/Log.h"

void ECS::Model::LoadModel(std::string p_path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(p_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		MYTH_ERROR("ASSIMP::%s", importer.GetErrorString());
		return;
	}

	m_directory = p_path.substr(0, p_path.find_last_of('/'));
	ProcessNode(p_path, scene->mRootNode, scene);
}

void ECS::Model::ProcessNode(const std::string& p_nodeName, aiNode* p_node, const aiScene* p_scene)
{
	for (unsigned int i = 0; i < p_node->mNumMeshes; i++)
	{
		aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		std::shared_ptr<Part> part = std::make_shared<Part>();
		Ressources::RessourcesManager& rm = Core::System::Application::GetInstance().GetRessourcesManager();
		m_parts.push_back(std::move(part));

		m_parts.back()->mesh = ProcessMesh((p_nodeName + "_mesh[" + std::to_string(i) + "]"), mesh, p_scene, rm);
		rm.RegisterAndLoadRessource(m_parts.back()->mesh);
	}

	for (unsigned int i = 0; i < p_node->mNumChildren; i++)
	{
		ProcessNode((p_nodeName + "_child[" + std::to_string(i) + "]"), p_node->mChildren[i], p_scene);
	}
}

void ProcessVertex(Vertex& p_vertex, aiMesh* p_mesh, unsigned int p_indice)
{
	p_vertex.position.m_x = p_mesh->mVertices[p_indice].x;
	p_vertex.position.m_y = p_mesh->mVertices[p_indice].y;
	p_vertex.position.m_z = p_mesh->mVertices[p_indice].z;

	p_vertex.normal.m_x = p_mesh->mNormals[p_indice].x;
	p_vertex.normal.m_y = p_mesh->mNormals[p_indice].y;
	p_vertex.normal.m_z = p_mesh->mNormals[p_indice].z;

	if (p_mesh->mTextureCoords[0])
	{
		p_vertex.uv.m_x = p_mesh->mTextureCoords[0][p_indice].x;
		p_vertex.uv.m_y = p_mesh->mTextureCoords[0][p_indice].y;
	}
	else
		p_vertex.uv = Vec2{ 0.f, 0.f };
}

Ressources::Mesh* ECS::Model::ProcessMesh(const std::string& p_meshName, aiMesh* p_mesh, const aiScene* p_scene, Ressources::RessourcesManager& rm)
{
	Ressources::Mesh* tmp_mesh = new Ressources::Mesh(p_meshName);
	std::vector<Ressources::Texture*> textures;

	for (unsigned int i = 0; i < p_mesh->mNumVertices; i++)
	{
		Vertex vertex;
		ProcessVertex(vertex, p_mesh, i);
		tmp_mesh->m_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < p_mesh->mNumFaces; i++)
	{
		aiFace face = p_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			tmp_mesh->m_indices.push_back(face.mIndices[j]);
	}

	if (p_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];
		Ressources::Material* mat = new Ressources::Material(material->GetName().C_Str());

		std::vector<Ressources::Texture*> diffuseMaps;
		LoadMaterialTextures(p_meshName, material, aiTextureType_DIFFUSE, "texture_diffuse", diffuseMaps);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		if (diffuseMaps.size() != 0)
			mat->m_diffuseTexture = diffuseMaps[0];

		std::vector<Ressources::Texture*> specularMaps;
		LoadMaterialTextures(p_meshName, material, aiTextureType_SPECULAR, "texture_specular", specularMaps);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		if (specularMaps.size() != 0)
			mat->m_specularTexture = specularMaps[0];

		rm.RegisterAndLoadRessource(mat);
		m_parts.back()->material = mat;
	}

	tmp_mesh->m_gpu = std::make_shared<GPUMesh>();
	tmp_mesh->m_gpu->VertexCount = tmp_mesh->m_vertices.size();
	tmp_mesh->m_gpu->offset = m_allVertices.size();
	m_allVertices.insert(m_allVertices.end(), tmp_mesh->m_vertices.begin(), tmp_mesh->m_vertices.end());
	return tmp_mesh;
}

void ECS::Model::LoadMaterialTextures(const std::string& p_meshName, aiMaterial* p_mat, aiTextureType p_type,
									  const std::string& p_typeName, std::vector<Ressources::Texture*>& p_textures)
{
	for (unsigned int i = 0; i < p_mat->GetTextureCount(p_type); i++)
	{
		aiString str;
		p_mat->GetTexture(p_type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_loadedTextures.size(); j++)
		{
			if (std::strcmp(m_loadedTextures[j]->m_path.data(), str.C_Str()) == 0)
			{
				p_textures.push_back(m_loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Ressources::Texture* texture = new Ressources::Texture(p_meshName + "_" + p_typeName + "[" + std::to_string(i) + "]");
			texture->m_type = p_typeName;
			texture->m_path = m_directory + '/' + std::string(str.C_Str());
			Ressources::RessourcesManager& rm = Core::System::Application::GetInstance().GetRessourcesManager();
			p_textures.push_back(texture);
			m_loadedTextures.push_back(p_textures.back());
			rm.RegisterAndLoadRessource(m_loadedTextures.back());
		}
	}

	return;
}

void ECS::Model::Draw(Core::GameObject* p_gameObject)
{
	for (std::shared_ptr<Part> part : m_parts)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_shader->Program, "uModel"), 1, GL_FALSE, m_matrix.m_mat.data());
		glBindTexture(GL_TEXTURE_2D, part->material->m_diffuseTexture->m_gpu->data);
		glDrawArrays(GL_TRIANGLES, part->mesh->m_gpu->offset, part->mesh->m_gpu->VertexCount);
	}
}
