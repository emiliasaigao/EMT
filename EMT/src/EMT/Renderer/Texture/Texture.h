#pragma once
#include "EMT/EMTEnum.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"

namespace EMT {
	struct TextureSettings
	{
		// Texture format
		int TextureFormat = EMT_NONE; // If set to EMT_NONE, the data format will be used

		/* isSRGB will let the loader know that the texture needs to be "linearlized" before it is sampled in the shaders (i.e the texture is in a non liner space)
		.Note: If you generate your own data and it is already in linear space (like light probes), be careful */
		bool IsSRGB = false;

		// Texture wrapping options
		int TextureWrapSMode = EMT_REPEAT;
		int TextureWrapTMode = EMT_REPEAT;
		bool HasBorder = false;
		glm::vec4 BorderColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); //default border used for shadow map

		// Texture filtering options
		int TextureMinificationFilterMode = EMT_LINEAR_MIPMAP_LINEAR; // Filtering mode when the texture moves further away and multiple texels map to one pixel (trilinear for best quality)
		int TextureMagnificationFilterMode = EMT_LINEAR; // Filtering mode when the texture gets closer and multiple pixels map to a single texel (Never needs to be more than bilinear because that is as accurate as it gets in this situation)

		// Mip options
		bool HasMips = true;
		int MipBias = 0; // positive means blurrier texture selected, negative means sharper texture which can show texture aliasing

		// Record data type for framebuffer resizing
		int dataFormat = EMT_NONE;
		int dataType = EMT_NONE;

		int ChannelNum = 0;
	};

	class Texture
	{
	public:
		static Ref<Texture> Create(const TextureSettings& settings = TextureSettings());
		~Texture() {}

		// 生成texture
		virtual void Generate2DTexture(unsigned int width,
			unsigned int height, int dataFormat,
			int pixelDataType = EMT_UNSIGNED_BYTE,
			const void* data = nullptr) = 0;
		virtual void Generate2DMultisampleTexture(unsigned int width, unsigned int height) = 0;
		// 尝试生成MipMap，只有在被Generate之后有用
		virtual void GenerateMips() = 0;
		virtual void CopyDataFormFBO2D(unsigned int level, unsigned int xoffset, unsigned int yoffset, unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

		virtual void Bind(int unit = 0) = 0;
		virtual void Unbind() = 0;
		virtual void DisplayTexture(bool invert = true) = 0;

		// 两个函数只能在Generate之前使用
		virtual inline void SetTextureSettings(TextureSettings settings) = 0;
		virtual inline void SetTextureFormat(int format) = 0;

		// Getters
		virtual inline unsigned int GetTextureId() const = 0;
		virtual inline unsigned int GetTextureTarget() const = 0;
		virtual inline bool IsGenerated() const = 0;
		virtual inline unsigned int GetWidth() const = 0;
		virtual inline unsigned int GetHeight() const = 0;
		virtual inline TextureSettings& GetTextureSettings() = 0;

	};

	// 抽象的Texture基类
	//class Texture {
	//public:
	//	virtual unsigned int GetHeight() const = 0;
	//	virtual unsigned int GetWidth() const = 0;
	//	virtual void Bind(unsigned int slot = 0) const = 0;
	//	virtual ~Texture() {}
	//};

	//class Texture2D : public Texture {
	//public:
	//	static Ref<Texture2D> Create(const std::string& path);
	//};
}