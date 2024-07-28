#include "emtpch.h"
#include "Material.h"

#include "EMT/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLMaterial.h"

namespace EMT {
	Ref<Material> Material::Create(const Ref<Texture>& albedoMap, const Ref<Texture>& normalMap, const Ref<Texture>& metallicMap, const Ref<Texture>& roughnessMap,
		const Ref<Texture>& ambientOcclusionMap, const Ref<Texture>& mixtureMap) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "现在还不支持RenderAPI::None"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLMaterial>(albedoMap, normalMap, metallicMap, roughnessMap, ambientOcclusionMap, mixtureMap);
		}
		EMT_CORE_ASSERT(false, "尚未选择RenderAPI");
		return nullptr;
	}

}
