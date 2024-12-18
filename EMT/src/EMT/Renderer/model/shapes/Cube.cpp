#include "emtpch.h"
#include "Cube.h"

namespace EMT {
	Cube::Cube()
	{
		m_Positions = esgstl::vector<glm::vec3>
		{
			// back face
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f,  1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f,  1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f,  1.0f, -1.0f),

			// front face
			glm::vec3(-1.0f, -1.0f,  1.0f),
			glm::vec3(1.0f, -1.0f,  1.0f),
			glm::vec3(1.0f,  1.0f,  1.0f),
			glm::vec3(1.0f,  1.0f,  1.0f),
			glm::vec3(-1.0f,  1.0f,  1.0f),
			glm::vec3(-1.0f, -1.0f,  1.0f),

			// left face
			glm::vec3(-1.0f,  1.0f,  1.0f),
			glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f,  1.0f),
			glm::vec3(-1.0f,  1.0f,  1.0f),

			// right face
			glm::vec3(1.0f,  1.0f,  1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f,  1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f,  1.0f,  1.0f),
			glm::vec3(1.0f, -1.0f,  1.0f),

			// bottom face
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f,  1.0f),
			glm::vec3(1.0f, -1.0f,  1.0f),
			glm::vec3(-1.0f, -1.0f,  1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),

			// top face
			glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3(1.0f,  1.0f,  1.0f),
			glm::vec3(1.0f,  1.0f, -1.0f),
			glm::vec3(1.0f,  1.0f,  1.0f),
			glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3(-1.0f,  1.0f,  1.0f)
		};

		m_Normals = esgstl::vector<glm::vec3>
		{
			// back face
			glm::vec3(0.0f,  0.0f, -1.0f),
			glm::vec3(0.0f,  0.0f, -1.0f),
			glm::vec3(0.0f,  0.0f, -1.0f),
			glm::vec3(0.0f,  0.0f, -1.0f),
			glm::vec3(0.0f,  0.0f, -1.0f),
			glm::vec3(0.0f,  0.0f, -1.0f),

			// front face
			glm::vec3(0.0f,  0.0f,  1.0f),
			glm::vec3(0.0f,  0.0f,  1.0f),
			glm::vec3(0.0f,  0.0f,  1.0f),
			glm::vec3(0.0f,  0.0f,  1.0f),
			glm::vec3(0.0f,  0.0f,  1.0f),
			glm::vec3(0.0f,  0.0f,  1.0f),

			// left face
			glm::vec3(-1.0f,  0.0f,  0.0f),
			glm::vec3(-1.0f,  0.0f,  0.0f),
			glm::vec3(-1.0f,  0.0f,  0.0f),
			glm::vec3(-1.0f,  0.0f,  0.0f),
			glm::vec3(-1.0f,  0.0f,  0.0f),
			glm::vec3(-1.0f,  0.0f,  0.0f),

			// right face
			glm::vec3(1.0f,  0.0f,  0.0f),
			glm::vec3(1.0f,  0.0f,  0.0f),
			glm::vec3(1.0f,  0.0f,  0.0f),
			glm::vec3(1.0f,  0.0f,  0.0f),
			glm::vec3(1.0f,  0.0f,  0.0f),
			glm::vec3(1.0f,  0.0f,  0.0f),

			// bottom face
			glm::vec3(0.0f, -1.0f,  0.0f),
			glm::vec3(0.0f, -1.0f,  0.0f),
			glm::vec3(0.0f, -1.0f,  0.0f),
			glm::vec3(0.0f, -1.0f,  0.0f),
			glm::vec3(0.0f, -1.0f,  0.0f),
			glm::vec3(0.0f, -1.0f,  0.0f),

			// top face
			glm::vec3(0.0f,  1.0f,  0.0f),
			glm::vec3(0.0f,  1.0f,  0.0f),
			glm::vec3(0.0f,  1.0f,  0.0f),
			glm::vec3(0.0f,  1.0f,  0.0f),
			glm::vec3(0.0f,  1.0f,  0.0f),
			glm::vec3(0.0f,  1.0f,  0.0f)
		};

		m_TexCoords = {
			// back face
			glm::vec2(0.0f, 0.0f), // bottom-left
			glm::vec2(1.0f, 1.0f), // top-right
			glm::vec2(1.0f, 0.0f), // bottom-right         
			glm::vec2(1.0f, 1.0f), // top-right
			glm::vec2(0.0f, 0.0f), // bottom-left
			glm::vec2(0.0f, 1.0f), // top-left

			// front face
			glm::vec2(0.0f, 0.0f), // bottom-left
			glm::vec2(1.0f, 0.0f), // bottom-right
			glm::vec2(1.0f, 1.0f), // top-right
			glm::vec2(1.0f, 1.0f), // top-right
			glm::vec2(0.0f, 1.0f), // top-left
			glm::vec2(0.0f, 0.0f), // bottom-left

			// left face
			glm::vec2(1.0f, 0.0f), // top-right
			glm::vec2(1.0f, 1.0f), // top-left
			glm::vec2(0.0f, 1.0f), // bottom-left
			glm::vec2(0.0f, 1.0f), // bottom-left
			glm::vec2(0.0f, 0.0f), // bottom-right
			glm::vec2(1.0f, 0.0f), // top-right

			// right face
			glm::vec2(1.0f, 0.0f), // top-left
			glm::vec2(0.0f, 1.0f), // bottom-right
			glm::vec2(1.0f, 1.0f), // top-right         
			glm::vec2(0.0f, 1.0f), // bottom-right
			glm::vec2(1.0f, 0.0f), // top-left
			glm::vec2(0.0f, 0.0f), // bottom-left

			// bottom face
			glm::vec2(0.0f, 1.0f), // top-right
			glm::vec2(1.0f, 1.0f), // top-left
			glm::vec2(1.0f, 0.0f), // bottom-left
			glm::vec2(1.0f, 0.0f), // bottom-left
			glm::vec2(0.0f, 0.0f), // bottom-right
			glm::vec2(0.0f, 1.0f), // top-right

			// top face
			glm::vec2(0.0f, 1.0f), // top-left
			glm::vec2(1.0f, 0.0f), // bottom-right
			glm::vec2(1.0f, 1.0f), // top-right     
			glm::vec2(1.0f, 0.0f), // bottom-right
			glm::vec2(0.0f, 1.0f), // top-left
			glm::vec2(0.0f, 0.0f)  // bottom-left 
		};

		m_Tangents = {
			// back face
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			// front face
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			// left face
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),

			// right face
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),

			// bottom face
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			// top face
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		};

		m_Bitangents = {
			// back face
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			// front face
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			// left face
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			// right face
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			// bottom face
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),

			// top face
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		SetupMesh();
	}

	Cube::~Cube() {}
}