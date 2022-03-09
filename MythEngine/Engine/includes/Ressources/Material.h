#pragma once
#include "Ressources/Texture.h"
#include "maths/Vec4.hpp"

namespace Ressources
{
	class Material : public Ressource
	{
	public:
		Material(const std::string& p_name) : Ressource(p_name) {};
		~Material() {};

		Vec4 m_diffuseColor;
		Vec4 m_specularColor;
		Texture* m_diffuseTexture = nullptr;
		Texture* m_specularTexture = nullptr;

		const std::type_info& GetRessourceType() const override { return typeid(*this); };
	};
}