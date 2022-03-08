#pragma once
#include "Ressources/Ressource.h"
#include "glad/glad.h"

namespace Ressources
{
	class Shader : public Ressource
	{
	public :
		Shader(const std::string& name, const char* p_vertexShader, const char* p_fragShader);
		
		GLuint Program;

		const std::type_info& GetRessourceType() const override { return typeid(*this); };
		GLuint CompileShader(GLenum ShaderType, const char* shaderStrs);
		void use();
	};
}