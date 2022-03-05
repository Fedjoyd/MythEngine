#pragma once
#include "Ressources/Ressource.h"
#include "maths/Vec3.hpp"
#include <vector>

struct Vertex
{
	Vec3 position;
	Vec2 uv;
	Vec3 normal;
};

struct GPUMesh
{
	unsigned int VertexCount;
	unsigned int offset;
};

namespace Ressources
{
	class Mesh : Ressources::Ressource
	{
	public:
		Mesh(const std::string& p_name) : Ressource(p_name) {};
		~Mesh() {};

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		GPUMesh* m_gpu = nullptr;

		const std::type_info& GetRessourceType() const override { return typeid(*this); };
	};
}