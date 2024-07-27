#include "emtpch.h"
#include "FrameBuffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace EMT {
    Ref<FrameBuffer> EMT::FrameBuffer::Create(int width, int height, bool isMultiSample)
    {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "现在还不支持RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:	    return std::make_shared<OpenGLFrameBuffer>(width, height, isMultiSample);
		}
		EMT_CORE_ASSERT(false, "尚未选择RenderAPI");
		return nullptr;
    }
}

