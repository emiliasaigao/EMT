#include "emtpch.h"
#include "OpenGLVertexArray.h"
#include <Glad/glad.h>
#include <GLFW/glfw3.h>

namespace EMT {
	static GLenum ShaderDataType2OpenGLDataType(ShaderDataType type) {
		switch (type)
		{
			case EMT::ShaderDataType::None:		return GL_NONE;
			case EMT::ShaderDataType::Float:	return GL_FLOAT;
			case EMT::ShaderDataType::Float2:	return GL_FLOAT;
			case EMT::ShaderDataType::Float3:	return GL_FLOAT;
			case EMT::ShaderDataType::FLoat4:	return GL_FLOAT;
			case EMT::ShaderDataType::Int:		return GL_INT;
			case EMT::ShaderDataType::Int2:		return GL_INT;
			case EMT::ShaderDataType::Int3:		return GL_INT;
			case EMT::ShaderDataType::Int4:		return GL_INT;
			case EMT::ShaderDataType::Mat3:		return GL_FLOAT;
			case EMT::ShaderDataType::Mat4:		return GL_FLOAT;
			case EMT::ShaderDataType::Bool:		return GL_BOOL;
		}
		EMT_CORE_ASSERT(false, "未知类型!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1,&m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		EMT_CORE_ASSERT(vertexBuffer->GetLayout().GetBufferElements().size(), "vertexBuffer还没有设置布局！")
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		const auto& layout = vertexBuffer->GetLayout();
		unsigned int index = 0;
		for (auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataType2OpenGLDataType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStribe(),
				(const void*)element.Offset);
			++index;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer)
	{
		glBindVertexArray(m_RendererID);
		elementBuffer->Bind();
		m_ElementBuffer = elementBuffer;
	}
}