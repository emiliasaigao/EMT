#pragma once
#include "EMT/Renderer/VertexArray.h"

namespace EMT {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void UnBind() const override;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		void setElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer) override;

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		const std::shared_ptr<ElementBuffer>& GetElementBuffer() const override { return m_ElementBuffer; }

	private:
		unsigned int m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<ElementBuffer> m_ElementBuffer;
	};
}