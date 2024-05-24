#pragma once
#include "EMT/Renderer/Buffer.h"

namespace EMT {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer() {}
		OpenGLVertexBuffer(float* vertices, unsigned int size);
		virtual ~OpenGLVertexBuffer();
		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		unsigned int m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLElementBuffer : public ElementBuffer {
	public:
		OpenGLElementBuffer(unsigned int* indices, unsigned int count);
		virtual ~OpenGLElementBuffer();
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual unsigned int GetCount() const override { return m_count; }
	private:
		unsigned int m_RendererID;
		unsigned int m_count;
	};
}