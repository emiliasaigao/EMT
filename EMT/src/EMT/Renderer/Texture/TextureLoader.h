#pragma once
#include "Texture.h"
#include "Cubemap.h"
#include "stb_image/stb_image.h"

namespace esgstl {
	template <class CharType, class CharTraits>
	struct hash<std::basic_string<CharType, CharTraits>> {
		size_t operator()(const std::basic_string<CharType, CharTraits>& str) const noexcept {
			return bitwise_hash((const unsigned char*)str.c_str(),
				str.size() * sizeof(CharType));
		}
	};
}

namespace EMT {

	class TextureLoader
	{
	public:
		static void InitDefaultTextures();

		
		static Ref<Texture> Load2DTexture(const std::string& path, TextureSettings* settings = nullptr);
		static Ref<Cubemap> LoadCubemapTexture(const esgstl::vector<std::string>& paths, CubemapSettings* settings = nullptr);

		static void DestroyCachedTexture();

		// getters
		static inline Ref<Texture> GetWhiteTexture() { return s_WhiteTexture; }
		static inline Ref<Texture> GetBlackTexture() { return s_BlackTexture; }
		static inline Ref<Texture> GetDefaultAlbedo() { return s_DefaultAlbedo; }
		static inline Ref<Texture> GetDefaultNormal() { return s_DefaultNormal; }
		static inline Ref<Texture> GetDefaultMetallic() { return s_BlackTexture; }
		static inline Ref<Texture> GetDefaultRoughness() { return s_WhiteTexture; }
		static inline Ref<Texture> GetDefaultAO() { return s_WhiteTexture; }
		static inline Ref<Texture> GetFullMetallic() { return s_WhiteTexture; }
		static inline Ref<Texture> GetNoMetallic() { return s_BlackTexture; }
		static inline Ref<Texture> GetFullRoughness() { return s_WhiteTexture; }
		static inline Ref<Texture> GetNoRoughness() { return s_BlackTexture; }

	private:
		static esgstl::unordered_map<std::string, Ref<Texture>> mTextureCache;

		// ≤‚ ‘”√ƒ¨»œÃ˘Õº
		static Ref<Texture> s_DefaultAlbedo;
		static Ref<Texture> s_DefaultNormal;
		static Ref<Texture> s_WhiteTexture;
		static Ref<Texture> s_BlackTexture;
	};
}