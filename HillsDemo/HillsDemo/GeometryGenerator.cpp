#include "GeometryGenerator.h"

void DXGraphicAPI::GeometryGenerator::CreateGrid(float width, float depth, size_t m, size_t n, MeshData& meshData)
{
	size_t vertexCount = (m * n);
	size_t faceCount = (m - 1) * (n - 1) * 2;

	float halfwidth = width * 0.5f;
	float halfdepth = depth * 0.5f;

	float dx = width / (m - 1);
	float dy = depth / (n - 1);

	auto getHeight = [](const float x, const float y) -> float
	{
		return (10.0f * sinf(0.15f *x) * cosf(0.15f * y));
	};
	meshData.Clear();
	meshData.vertices.resize(vertexCount);
	for (size_t j = 0; j < n; ++j)
	{
		for (size_t i = 0; i < m; ++i)
		{
			float y = -halfdepth + j * dy;
			float x = -halfwidth + i * dx;
			float z = getHeight(x, y);
			meshData.vertices[i + j * m] = Math::Vector3(x, y, z);
		}
	}

	size_t k = 0;
	meshData.indices.resize(faceCount * 3);
	for (size_t j = 0; j < n - 1; ++j)
	{
		for (size_t i = 0; i < m - 1; ++i)
		{
			meshData.indices[k] = static_cast<unsigned int>(i + j * m);
			meshData.indices[k + 1] = static_cast<unsigned int>(i + 1 + j * m);
			meshData.indices[k + 2] = static_cast<unsigned int>(i + (j + 1) * m);
			meshData.indices[k + 3] = static_cast<unsigned int>(i + (j + 1) * m);
			meshData.indices[k + 4] = static_cast<unsigned int>(i + 1 + j * m);
			meshData.indices[k + 5] = static_cast<unsigned int>(i + 1 + (j + 1) * m);

			k += 6;
		}
	}
}