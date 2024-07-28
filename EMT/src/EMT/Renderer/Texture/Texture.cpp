#include "emtpch.h"
#include "Texture.h"
#include "EMT/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace EMT {

	Ref<Texture> Texture::Create(const TextureSettings& settings) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "���ڻ���֧��RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture>(settings);
		}
		EMT_CORE_ASSERT(false, "��δѡ��RenderAPI");
		return nullptr;
	}

}