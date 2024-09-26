#pragma once
#include "./Material.h"
#include "./glm/glm.hpp"
#include "./Triangle.h"
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
		Mesh(esgstl::vector<glm::vec3>& positions, 
			 esgstl::vector<glm::vec2>& uvs, 
			 esgstl::vector<glm::vec3>& normals, 
			 esgstl::vector<unsigned int>& indices);

		Mesh(esgstl::vector<glm::vec3>& positions, 
			 esgstl::vector<glm::vec2>& uvs, 
			 esgstl::vector<glm::vec3>& normals, 
			 esgstl::vector<glm::vec3>& tangents, 
			 esgstl::vector<glm::vec3>& bitangents, 
			 esgstl::vector<unsigned int>& indices);

		~Mesh() {}

		void Draw() const;
		void OnImGuiRender();

		inline bool HasTangents() const { return m_Tangents.size() > 0; }

		inline Ref<Material> GetMaterial() { return m_Material; }

		inline const AABB& GetAABB() { return m_BVH->WorldBound(); }
		inline void SetBVHNode(Ref<BVHBuildNode<Mesh>> node) {
			m_BVH_Node = node;
		}

	protected:
		void SetupMesh();

	protected:
		esgstl::vector<glm::vec3> m_Positions;
		esgstl::vector<glm::vec2> m_TexCoords;
		esgstl::vector<glm::vec3> m_Normals;
		esgstl::vector<glm::vec3> m_Tangents;
		esgstl::vector<glm::vec3> m_Bitangents;

		esgstl::vector<unsigned int> m_Indices;
		
		//unsigned int mVAO, mVBO, mEBO;
		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<ElementBuffer> m_EBO;
		Ref<Material> m_Material; 
		Ref<BVHAccel<Triangle>> m_BVH;
		Ref<BVHBuildNode<Mesh>> m_BVH_Node;
	};
}