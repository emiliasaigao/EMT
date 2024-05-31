#include "emtpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace EMT {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "���ڻ���֧��RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGLVertexArray();
		}
		EMT_CORE_ASSERT(false, "��δѡ��RenderAPI");
		return nullptr;
	}
}