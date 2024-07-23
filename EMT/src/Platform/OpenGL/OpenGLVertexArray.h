#pragma once
#include "EMT/Renderer/VertexArray.h"

namespace EMT {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void Unbind() const override;

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		void SetElementBuffer(const Ref<ElementBuffer>& elementBuffer) override;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		const Ref<ElementBuffer>& GetElementBuffer() const override { return m_ElementBuffer; }

	private:
		unsigned int m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<ElementBuffer> m_ElementBuffer;
	};
}