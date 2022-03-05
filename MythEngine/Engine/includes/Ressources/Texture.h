#pragma once
#include "Ressources/Ressource.h"
#include "glad/glad.h"

struct GPUTexture
{
	GLuint data;
};

namespace Ressources
{
	class Texture : Ressource
	{
	public :
		Texture(const std::string& p_name) : Ressource(p_name) {};
		~Texture() {};

		std::string m_type;
		std::string m_path;
		GPUTexture* m_gpu = nullptr;

		const std::type_info& GetRessourceType() const override { return typeid(*this); };

		//void LoadRessource() override;
		//void UnloadRessource() override;
	};
}