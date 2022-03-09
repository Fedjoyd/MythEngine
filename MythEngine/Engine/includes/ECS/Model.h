#pragma once
#include "ECS/Component.h"
#include "Ressources/Mesh.h"
#include "Ressources/Texture.h"
#include "Ressources/Shader.h"
#include "Ressources/Material.h"
#include "Ressources/RessourcesManager.h"
#include "maths/Mat4x4.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Part
{
	Ressources::Mesh* mesh = nullptr;
	Ressources::Material* material = nullptr;
	Mat4x4 localMatrix = Mat4x4::Identity;
};

namespace ECS
{
	class Model : public Component
	{
	public:
		Model(const char* path)
		{
			LoadModel(path);
			m_shader = new Ressources::Shader("basicShader", "ressources/shaders/basicShader.vert", "ressources/shaders/basicShader.frag");
		};
		~Model() 
		{
			delete m_shader;
		};

		std::vector<Vertex> m_allVertices;
		Ressources::Shader* m_shader;
		Mat4x4 m_matrix = Mat4x4::Identity;

	private:
		std::vector<std::shared_ptr<Part>> m_parts;
		std::vector<Ressources::Texture*> m_loadedTextures;
		std::string m_directory;

		const std::type_info& GetComponentType() const override { return typeid(*this); };

		void LoadModel(std::string p_path);
		void ProcessNode(const std::string& p_nodeName, aiNode* p_node, const aiScene* p_scene);
		Ressources::Mesh* ProcessMesh(const std::string& p_meshName, aiMesh* p_mesh, const aiScene* p_scene, Ressources::RessourcesManager& rm);
		void LoadMaterialTextures(const std::string& p_meshName, aiMaterial* p_mat, aiTextureType p_type,
								  const std::string& p_typeName, std::vector<Ressources::Texture*>& p_textures);
		void Draw(Core::GameObject* p_gameObject) override;
	};
}