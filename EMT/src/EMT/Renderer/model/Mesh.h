#pragma once
#include "./Material.h"
#include "./glm/glm.hpp"
#include "EMT/Renderer/VertexArray.h"
#include "EMT/Renderer/Buffer.h"
#include "EMT/Renderer/RenderCommand.h"

namespace EMT {
	struct Vertex {
		// 坐标
		glm::vec3 Position;
		// 法向量
		glm::vec3 Normal;
		// 纹理坐标
		glm::vec2 TexCoords;
		// 切线t向量
		glm::vec3 Tangent;
		// 切线b向量
		glm::vec3 Bitangent;
	};

	class Model;

	class Mesh {
		friend Model;
	public:
		Mesh();
		Mesh(std::vector<glm::vec3>& positions, 
			 std::vector<glm::vec2>& uvs, 
			 std::vector<glm::vec3>& normals, 
			 std::vector<unsigned int>& indices);

		Mesh(std::vector<glm::vec3>& positions, 
			 std::vector<glm::vec2>& uvs, 
			 std::vector<glm::vec3>& normals, 
			 std::vector<glm::vec3>& tangents, 
			 std::vector<glm::vec3>& bitangents, 
			 std::vector<unsigned int>& indices);

		~Mesh() {}

		void Draw() const;
		void OnImGuiRender();

		inline bool HasTangents() const { return m_Tangents.size() > 0; }

		inline Material* GetMaterial() { return &m_Material; }

	protected:
		void SetupMesh();

	protected:
		std::vector<glm::vec3> m_Positions;
		std::vector<glm::vec2> m_TexCoords;
		std::vector<glm::vec3> m_Normals;
		std::vector<glm::vec3> m_Tangents;
		std::vector<glm::vec3> m_Bitangents;

		std::vector<unsigned int> m_Indices;
		
		//unsigned int mVAO, mVBO, mEBO;
		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<ElementBuffer> m_EBO;
		Material m_Material; 
	};
}