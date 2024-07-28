#pragma once
#include "EMT/Renderer/model/Material.h"
#include "glad/glad.h"

namespace EMT {
	class OpenGLMaterial : public Material {
	public:
		OpenGLMaterial(const Ref<Texture>& albedoMap, const Ref<Texture>& normalMap, const Ref<Texture>& metallicMap, const Ref<Texture>& roughnessMap,
			const Ref<Texture>& ambientOcclusionMap, const Ref<Texture>& mixtureMap);
		~OpenGLMaterial() {}

		virtual void BindMaterial(const Ref<Shader>& shader) const override;
		virtual void OnImGuiRender() override;

		virtual void SeperateMixture() override;

		virtual inline void SetAlbedoMap(Ref<Texture> texture) override  { m_AlbedoMap = texture; }
		virtual inline void SetNormalMap(Ref<Texture> texture) override { m_NormalMap = texture; }
		virtual inline void SetMetallicMap(Ref<Texture> texture) override { m_MetallicMap = texture; }
		virtual inline void SetRoughnessMap(Ref<Texture> texture) override { m_RoughnessMap = texture; }
		virtual inline void SetMixtureMap(Ref<Texture> texture) { m_MixtureMap = texture; }

		virtual inline void SetAmbientOcclusionMap(Ref<Texture> texture) override { m_AOMap = texture; }
	private:
		Ref<Texture> m_AlbedoMap;
		Ref<Texture> m_NormalMap;
		Ref<Texture> m_MetallicMap;
		Ref<Texture> m_RoughnessMap;
		Ref<Texture> m_AOMap;
		// ◊®√≈’Î∂‘gltfÃ˘Õº
		Ref<Texture> m_MixtureMap;
	};
}