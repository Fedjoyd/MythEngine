#include "ECS/Model.h"
#include <stb_image.h>

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
		m_meshes.push_back(ProcessMesh((p_nodeName + "_mesh[" + std::to_string(i) + "]"), mesh, scene));
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

Ressources::Mesh ECS::Model::ProcessMesh(const std::string& p_meshName, aiMesh* mesh, const aiScene* scene)
{
	Ressources::Mesh tmp_mesh(p_meshName);
	std::vector<Ressources::Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		ProcessVertex(vertex, mesh, i);
		tmp_mesh.m_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			tmp_mesh.m_indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Ressources::Texture> diffuseMaps = LoadMaterialTextures(p_meshName, material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Ressources::Texture> specularMaps = LoadMaterialTextures(p_meshName, material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	m_allVertices.insert(m_allVertices.end(), tmp_mesh.m_vertices.begin(), tmp_mesh.m_vertices.end());
	return tmp_mesh;
}

std::vector<Ressources::Texture> ECS::Model::LoadMaterialTextures(const std::string& p_meshName, aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Ressources::Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_loadedTextures.size(); j++)
		{
			if (std::strcmp(m_loadedTextures[j].m_path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Ressources::Texture texture(p_meshName + "_" + typeName + "[" + std::to_string(i) + "]");
			texture.m_gpu = new GPUTexture;
			texture.m_gpu->data = TextureFromFile(str.C_Str(), m_directory);
			texture.m_type = typeName;
			texture.m_path = m_directory + '/' + std::string(str.C_Str());
			textures.push_back(texture);
		}
	}

	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	int width, height, nrChannels;
	unsigned int textureID;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	glGenTextures(1, &textureID);

	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		MYTH_ERROR("Image loading failed on '%s'\n", path);
	}

	stbi_image_free(data);
	return textureID;
}