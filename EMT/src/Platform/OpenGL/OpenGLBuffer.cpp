#include "emtpch.h"
#include "OpenGLBuffer.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

namespace EMT {
	// VBO
	/************************************************************************************************/
	OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, unsigned int size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		m_Size = size;
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	/************************************************************************************************/

	// EBO
	/************************************************************************************************/
	OpenGLElementBuffer::OpenGLElementBuffer(unsigned int* indices, unsigned int count)
		: m_Count(count)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	OpenGLElementBuffer::~OpenGLElementBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLElementBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLElementBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	/************************************************************************************************/
}

