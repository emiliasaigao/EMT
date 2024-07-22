#include "emtpch.h"
#include "Texture.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace EMT {
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "���ڻ���֧��RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(path);
		}
		EMT_CORE_ASSERT(false, "��δѡ��RenderAPI");
		return nullptr;
	}
}