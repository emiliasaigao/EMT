#pragma once
#include "EMT/Renderer/Texture/TextureLoader.h"
#include "EMT/Renderer/Shader.h"

namespace EMT {
	class Material
	{
	public:
		Material(Ref<Texture> albedoMap = nullptr, Ref<Texture> normalMap = nullptr, Ref<Texture> metallicMap = nullptr, Ref<Texture> roughnessMap = nullptr,
			Ref<Texture> ambientOcclusionMap = nullptr, Ref<Texture> mixtureMap = nullptr);
		~Material() {}

		void SeperateMixture();
		void BindMaterial(const Ref<Shader>& shader) const;
		void OnImGuiRender();

		inline void SetAlbedoMap(Ref<Texture> texture) { m_AlbedoMap = texture; }
		inline void SetNormalMap(Ref<Texture> texture) { m_NormalMap = texture; }
		inline void SetMetallicMap(Ref<Texture> texture) { m_MetallicMap = texture; }
		inline void SetRoughnessMap(Ref<Texture> texture) { m_RoughnessMap = texture; }
		inline void SetAmbientOcclusionMap(Ref<Texture> texture) { m_AOMap = texture; }
		inline void SetMixtureMap(Ref<Texture> texture) { m_MixtureMap = texture; }

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