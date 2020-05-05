#pragma once
#include <vector>
#include "Vector.h"
namespace DXGraphicAPI
{
	class GeometryGenerator
	{
	public:
		GeometryGenerator() {}
		~GeometryGenerator() {}

		struct MeshData
		{
			std::vector<Math::Vector3> vertices;
			std::vector<unsigned int> indices;

			void Clear()
			{
				vertices.clear();
				indices.clear();
			}

		};

		void CreateGrid(float width, float depth, size_t m, size_t n, MeshData& meshData);
	};
}
