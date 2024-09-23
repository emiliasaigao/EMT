#include "emtpch.h"
#include "Mesh.h"

namespace EMT {
	Mesh::Mesh() {}

	Mesh::Mesh(esgstl::vector<glm::vec3>& positions,
		esgstl::vector<glm::vec2>& uvs,
		esgstl::vector<glm::vec3>& normals,
		esgstl::vector<unsigned int>& indices)
		: m_Positions(positions), m_TexCoords(uvs), m_Normals(normals), m_Indices(indices)
	{
	}

	Mesh::Mesh(esgstl::vector<glm::vec3>& positions,
		esgstl::vector<glm::vec2>& uvs,
		esgstl::vector<glm::vec3>& normals,
		esgstl::vector<glm::vec3>& tangents,
		esgstl::vector<glm::vec3>& bitangents,
		esgstl::vector<unsigned int>& indices)
		: m_Positions(positions), m_TexCoords(uvs), m_Normals(normals), m_Tangents(tangents), m_Bitangents(bitangents), m_Indices(indices)
	{
	}


	void Mesh::Draw() const {
		// 必须要先设定材质，否则材质里全是nullptr
		
		if (m_Indices.size() > 0) 
			RenderCommand::DrawIndexed(m_VAO);
		else 
			RenderCommand::DrawArray(m_VAO);
		m_VAO->Unbind();

	}


	void Mesh::SetupMesh() {
		// 计算包围盒用
		glm::vec3 min_vert = glm::vec3(std::numeric_limits<float>::infinity());
		glm::vec3 max_vert = glm::vec3(-std::numeric_limits<float>::infinity());

		esgstl::vector<Vertex> vertices;
		for (unsigned int i = 0;i < m_Positions.size();++i) {
			Vertex newVertex;
			if (m_Positions.size() > 0)
				newVertex.Position = m_Positions[i];
			if (m_Normals.size() > 0)
				newVertex.Normal = m_Normals[i];
			if (m_TexCoords.size() > 0)
				newVertex.TexCoords = m_TexCoords[i];
			if (m_Tangents.size() > 0)
				newVertex.Tangent = m_Tangents[i];
			if (m_Bitangents.size() > 0)
				newVertex.Bitangent = m_Bitangents[i];

			vertices.push_back(std::move(newVertex));
		}

		m_Material = Material::Create();
		m_VAO = VertexArray::Create();
		m_VBO = VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(Vertex));
		m_VBO->SetLayout({
			{EMT::ShaderDataType::Float3,"a_Pos"},
			{EMT::ShaderDataType::Float3,"a_Normal"},
			{EMT::ShaderDataType::Float2,"a_TexCoord"},
			{EMT::ShaderDataType::Float3,"a_Tangent"},
			{EMT::ShaderDataType::Float3,"a_Bitangent"}
		});
		
		m_VAO->AddVertexBuffer(m_VBO);
		
		esgstl::vector<Triangle> triangles;
		if (m_Indices.size() > 0) {
			m_EBO = ElementBuffer::Create(&m_Indices[0], m_Indices.size());
			m_VAO->SetElementBuffer(m_EBO);
			triangles.reserve(m_Indices.size() / 3);
			for (int i = 0; i < m_Indices.size(); i += 3) {
				triangles.emplace_back(m_Positions[m_Indices[i]], m_Positions[m_Indices[i + 1]], m_Positions[m_Indices[i + 2]]);
			}
		}
		else {
			triangles.reserve(m_Positions.size() / 3);
			for (int i = 0; i < m_Positions.size(); i += 3) {
				triangles.emplace_back(m_Positions[i], m_Positions[i + 1], m_Positions[i + 2]);
			}
		}
		m_BVH = std::make_shared<BVHAccel<Triangle>>(triangles);

		m_VAO->Unbind();
		
	}

	void Mesh::OnImGuiRender() {
		//show material
		if (ImGui::TreeNode("Material")) {
			m_Material->OnImGuiRender();
			ImGui::TreePop();
		}

	}
}