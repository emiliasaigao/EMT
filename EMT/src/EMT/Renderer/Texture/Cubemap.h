#pragma once
#include "EMT/EMTEnum.h"
#include "imgui/imgui.h"

namespace EMT {
	struct CubemapSettings
	{
		// Texture format
		int TextureFormat = EMT_NONE; // If set to GL_NONE, the data format will be used

		/* isSRGB will let the loader know that the texture needs to be "linearlized" before it is sampled in the shaders (ie the texture is in a non liner space)
		 * Anything that will be used for colour in a renderer should be linearlized. However textures that contain data (Heightfields, normal maps, metallic maps etc.) should not be,
		 * thus they are not in SRGB space. Note: If you generate your own data and it is already in linear space (like light probes), be careful */
		bool IsSRGB = false;

		// Texture wrapping options
		int TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		int TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		int TextureWrapRMode = EMT_CLAMP_TO_EDGE;

		// Texture filtering options
		int TextureMinificationFilterMode = EMT_LINEAR; // Filtering mode when the texture moves further away and multiple texels map to one pixel (trilinear for best quality)
		int TextureMagnificationFilterMode = EMT_LINEAR; // Filtering mode when the texture gets closer and multiple pixels map to a single texel (Never needs to be more than bilinear because that is as accurate as it gets in this sitation)

		// Mip Settings
		bool HasMips = false;
		int MipBias = 0; // positive means blurrier texture selected, negative means sharper texture which can show texture aliasing
	};
	class Cubemap
	{
	public:
		static Ref<Cubemap> Create(const CubemapSettings& settings = CubemapSettings());
		~Cubemap() {}

		virtual void Bind(int unit = 0) = 0;
		virtual void UnBind() = 0;

		virtual void GenerateCubemapFace(int face, unsigned int width, unsigned int height,
			int dataFormat, int pixelDataType = EMT_UNSIGNED_BYTE, 
			unsigned char* data = nullptr) = 0;

		virtual inline int GetWidth() = 0;
		virtual inline int GetHeight() = 0;
		virtual inline CubemapSettings* GetCubemapSettings() = 0;
		virtual unsigned int GetCubemapID() = 0;
		virtual inline unsigned int GetCubemapFaceTexID(int idx) = 0;
		virtual void DisplayTexture(bool invert = true) = 0;
		virtual inline void SetCubemapSettings(const CubemapSettings& settings) = 0;
	};
}