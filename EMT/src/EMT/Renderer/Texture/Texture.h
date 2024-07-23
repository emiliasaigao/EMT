#pragma once
#include "glad/glad.h"
#include "imgui/imgui.h"

namespace EMT {
	struct TextureSettings
	{
		// Texture format
		GLenum TextureFormat = GL_NONE; // If set to GL_NONE, the data format will be used

		/* isSRGB will let the loader know that the texture needs to be "linearlized" before it is sampled in the shaders (i.e the texture is in a non liner space)
		.Note: If you generate your own data and it is already in linear space (like light probes), be careful */
		bool IsSRGB = false;

		// Texture wrapping options
		GLenum TextureWrapSMode = GL_REPEAT;
		GLenum TextureWrapTMode = GL_REPEAT;
		bool HasBorder = false;
		glm::vec4 BorderColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); //default border used for shadow map

		// Texture filtering options
		GLenum TextureMinificationFilterMode = GL_LINEAR_MIPMAP_LINEAR; // Filtering mode when the texture moves further away and multiple texels map to one pixel (trilinear for best quality)
		GLenum TextureMagnificationFilterMode = GL_LINEAR; // Filtering mode when the texture gets closer and multiple pixels map to a single texel (Never needs to be more than bilinear because that is as accurate as it gets in this situation)

		// Mip options
		bool HasMips = true;
		int MipBias = 0; // positive means blurrier texture selected, negative means sharper texture which can show texture aliasing

		// Record data type for framebuffer resizing
		GLenum dataFormat = GL_NONE;
		GLenum dataType = GL_NONE;

		int ChannelNum = 0;
	};

	class Texture
	{
	public:
		Texture(const TextureSettings& settings = TextureSettings());
		~Texture();

		// 生成texture
		void Generate2DTexture(unsigned int width, unsigned int height, GLenum dataFormat, GLenum pixelDataType = GL_UNSIGNED_BYTE, const void* data = nullptr);
		void Generate2DMultisampleTexture(unsigned int width, unsigned int height);
		// 尝试生成MipMap，只有在被Generate之后有用
		void GenerateMips(); 

		void Bind(int unit = 0);
		void Unbind();
		void DisplayTexture(bool invert = true);

		// 两个函数只能在Generate之前使用
		inline void SetTextureSettings(TextureSettings settings) { m_TextureSettings = settings; }
		inline void SetTextureFormat(GLenum format) { m_TextureSettings.TextureFormat = format; }

		// Getters
		inline unsigned int GetTextureId() const { return m_RendererID; }
		inline unsigned int GetTextureTarget() const { return m_TextureTarget; }
		inline bool IsGenerated() const { return m_RendererID != 0; }
		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		inline TextureSettings& GetTextureSettings() { return m_TextureSettings; }

	private:
		// 应用Texture的设置
		void ApplyTextureSettings();
	private:
		GLenum m_TextureTarget;
		unsigned int m_RendererID;
		unsigned int m_Width, m_Height;

		TextureSettings m_TextureSettings;
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