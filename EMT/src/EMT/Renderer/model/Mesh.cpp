#include "emtpch.h"
#include "Mesh.h"

namespace EMT {
	Mesh::Mesh() {}

	Mesh::Mesh(std::vector<glm::vec3>& positions,
		std::vector<glm::vec2>& uvs,
		std::vector<glm::vec3>& normals,
		std::vector<unsigned int>& indices)
		: m_Positions(positions), m_TexCoords(uvs), m_Normals(normals), m_Indices(indices)
	{
	}

	Mesh::Mesh(std::vector<glm::vec3>& positions,
		std::vector<glm::vec2>& uvs,
		std::vector<glm::vec3>& normals,
		std::vector<glm::vec3>& tangents,
		std::vector<glm::vec3>& bitangents,
		std::vector<unsigned int>& indices)
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

		std::vector<Vertex> vertices;
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
			
		if (m_Indices.size() > 0) {
			m_EBO = ElementBuffer::Create(&m_Indices[0], m_Indices.size());
			m_VAO->SetElementBuffer(m_EBO);
		}

		m_VAO->Unbind();

	}

	void Mesh::OnImGuiRender() {
		//show material
		if (ImGui::TreeNode("Material")) {
			m_Material.OnImGuiRender();
			ImGui::TreePop();
		}

	}
}