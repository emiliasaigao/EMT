#include "emtpch.h"
#include "FrameBuffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace EMT {
    Ref<FrameBuffer> EMT::FrameBuffer::Create(int width, int height, bool isMultiSample)
    {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "���ڻ���֧��RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:	    return std::make_shared<OpenGLFrameBuffer>(width, height, isMultiSample);
		}
		EMT_CORE_ASSERT(false, "��δѡ��RenderAPI");
		return nullptr;
    }
}

