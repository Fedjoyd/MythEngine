#pragma once
#include "ECS/Component.h"
#include "Ressources/Mesh.h"
#include "Ressources/Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const char* path, const std::string& directory);

namespace ECS
{
	class Model : Component
	{
	public:
		Model(const char* path)
		{
			LoadModel(path);
		};
		~Model() {};

		std::vector<Vertex> m_allVertices;

	private:
		std::vector<Ressources::Mesh> m_meshes;
		std::vector<Ressources::Texture> m_loadedTextures;
		std::string m_directory;

		const std::type_info& GetComponentType() const override { return typeid(*this); };

		void LoadModel(std::string p_path);
		void ProcessNode(const std::string& p_nodeName, aiNode* p_node, const aiScene* p_scene);
		Ressources::Mesh ProcessMesh(const std::string& p_meshName, aiMesh* p_mesh, const aiScene* p_scene);
		std::vector<Ressources::Texture> LoadMaterialTextures(const std::string& p_meshName, aiMaterial* p_mat, aiTextureType p_type, const std::string& p_typeName);
	};
}