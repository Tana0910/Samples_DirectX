#pragma once 
#include <vector>
namespace Model
{

	template <class TVertex, class TSurface>
	class Model
	{
	private:
		int m_index;
		std::vector<TVertex> m_Vertices;
		std::vector<TSurface> m_Surfaces;
		void ArrangeToRenderingData();
	};

}
