#pragma once
namespace Model
{
	template <class TVector3>
	class Vertex
	{
	private:
		TVector3 m_vec;
		float m_color[4];
		int m_index;
	};
}