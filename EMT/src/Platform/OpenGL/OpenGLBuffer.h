#pragma once
#include "EMT/Renderer/Buffer.h"

namespace EMT {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer() {}
		OpenGLVertexBuffer(void* vertices, unsigned int size);
		virtual ~OpenGLVertexBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual unsigned int GetCount() const override { return m_Count; }
		virtual void SetLayout(const BufferLayout& layout) override { 
			m_Layout = layout; 
			m_Count = m_Size / layout.GetStribe();
		}

	private:
		unsigned int m_RendererID;
		unsigned int m_Size;
		unsigned int m_Count;
		BufferLayout m_Layout;
	};

	class OpenGLElementBuffer : public ElementBuffer {
	public:
		OpenGLElementBuffer(unsigned int* indices, unsigned int count);
		virtual ~OpenGLElementBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual unsigned int GetCount() const override { return m_Count; }
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;
	};
}