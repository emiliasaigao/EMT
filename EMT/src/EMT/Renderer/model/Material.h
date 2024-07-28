#pragma once
#include "EMT/Renderer/Texture/TextureLoader.h"
#include "EMT/Renderer/Shader.h"

namespace EMT {
	class Material {
	public:
		static Ref<Material> Create(const Ref<Texture>& albedoMap = nullptr,const Ref<Texture>& normalMap = nullptr,
			const Ref<Texture>& metallicMap = nullptr, const Ref<Texture>& roughnessMap = nullptr,
			const Ref<Texture>& ambientOcclusionMap = nullptr, const Ref<Texture>& mixtureMap = nullptr);
		
		~Material() {}

		virtual void BindMaterial(const Ref<Shader>& shader) const = 0;
		virtual void OnImGuiRender() = 0;

		virtual void SeperateMixture() = 0;

		virtual inline void SetAlbedoMap(Ref<Texture> texture) = 0;
		virtual inline void SetNormalMap(Ref<Texture> texture) = 0;
		virtual inline void SetMetallicMap(Ref<Texture> texture) = 0;
		virtual inline void SetRoughnessMap(Ref<Texture> texture) = 0;
		virtual inline void SetAmbientOcclusionMap(Ref<Texture> texture) = 0;
		virtual inline void SetMixtureMap(Ref<Texture> texture) = 0;
	};
}