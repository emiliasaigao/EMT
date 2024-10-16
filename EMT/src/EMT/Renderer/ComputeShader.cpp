#include "emtpch.h"
#include "ComputeShader.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLComputeShader.h"
namespace EMT {
    Ref<ComputeShader> EMT::ComputeShader::Create(const std::string& computePath)
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "现在还不支持RenderAPI::None"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLComputeShader>(computePath);
		}
		EMT_CORE_ASSERT(false, "尚未选择RenderAPI");
		return nullptr;
    }
}
