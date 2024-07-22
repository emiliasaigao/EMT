#include "emtpch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace EMT {
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "现在还不支持RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}
		EMT_CORE_ASSERT(false, "尚未选择RenderAPI");
		return nullptr;
	}

	Ref<ElementBuffer> ElementBuffer::Create(unsigned int* indices, unsigned int count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "现在还不支持RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLElementBuffer>(indices, count);
		}
		EMT_CORE_ASSERT(false, "尚未选择RenderAPI");
		return nullptr;
	}
}