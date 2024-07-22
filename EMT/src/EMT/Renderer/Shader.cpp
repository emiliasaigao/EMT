#include "emtpch.h"
#include "Shader.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace EMT {
	Ref<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "现在还不支持RenderAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(vertexPath, fragmentPath, geometryPath);
		}
		EMT_CORE_ASSERT(false, "尚未选择RenderAPI");
		return nullptr;
	}
}