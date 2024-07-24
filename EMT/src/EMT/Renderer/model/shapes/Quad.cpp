#include "emtpch.h"
#include "Quad.h"
namespace EMT {
	Quad::Quad()
	{
		m_Positions.push_back(glm::vec3(-1, 1, 0));
		m_Positions.push_back(glm::vec3(1, 1, 0));
		m_Positions.push_back(glm::vec3(-1, -1, 0));
		m_Positions.push_back(glm::vec3(1, -1, 0));
		m_TexCoords.push_back(glm::vec2(0, 1));
		m_TexCoords.push_back(glm::vec2(1, 1));
		m_TexCoords.push_back(glm::vec2(0, 0));
		m_TexCoords.push_back(glm::vec2(1, 0));
		m_Normals.push_back(glm::vec3(0, 0, 1));
		m_Normals.push_back(glm::vec3(0, 0, 1));
		m_Normals.push_back(glm::vec3(0, 0, 1));
		m_Normals.push_back(glm::vec3(0, 0, 1));

		//mTangents.push_back(glm::vec3(1, 0, 0));
		//mTangents.push_back(glm::vec3(1, 0, 0));
		//mTangents.push_back(glm::vec3(1, 0, 0));
		//mTangents.push_back(glm::vec3(1, 0, 0));
		//mBitangents.push_back(glm::vec3(0, 1, 0));
		//mBitangents.push_back(glm::vec3(0, 1, 0));
		//mBitangents.push_back(glm::vec3(0, 1, 0));
		//mBitangents.push_back(glm::vec3(0, 1, 0));

		m_Indices.push_back(1); m_Indices.push_back(0); m_Indices.push_back(2);
		m_Indices.push_back(3); m_Indices.push_back(1); m_Indices.push_back(2);

		SetupMesh();
	}

	Quad::~Quad()
	{
	}
}