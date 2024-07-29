#include "emtpch.h"
#include "GBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLGBuffer.h"

namespace EMT {
	Ref<GBuffer> GBuffer::Create(int width, int height) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "现在还不支持RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:	    return std::make_shared<OpenGLGBuffer>(width, height);
		}
		EMT_CORE_ASSERT(false, "尚未选择RenderAPI");
		return nullptr;
	}
}
