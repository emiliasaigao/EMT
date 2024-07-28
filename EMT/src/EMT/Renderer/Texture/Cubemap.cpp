#include "emtpch.h"
#include "Cubemap.h"
#include "EMT/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLCubemap.h"

namespace EMT {
	Ref<Cubemap> Cubemap::Create(const CubemapSettings& settings) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "现在还不支持RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLCubemap>(settings);
		}
		EMT_CORE_ASSERT(false, "尚未选择RenderAPI");
		return nullptr;
	}
}
