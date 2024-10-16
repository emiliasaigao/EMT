#pragma once
#include "EMT/Renderer/Texture/Texture.h"
#include "glad/glad.h"

namespace EMT {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const TextureSettings& settings);
		~OpenGLTexture();

		// 生成texture
		virtual void Generate2DTexture(unsigned int width, unsigned int height, int dataFormat, int pixelDataType = EMT_UNSIGNED_BYTE, const void* data = nullptr) override;
		virtual void Generate2DMultisampleTexture(unsigned int width, unsigned int height) override;
		virtual void Genarate2DTextureArray(unsigned int width, unsigned int height, int dataFormat, int pixelDataType = EMT_UNSIGNED_BYTE, const void* data = nullptr) override;

		// 尝试生成MipMap，只有在被Generate之后有用
		virtual void GenerateMips();
		virtual void CopyDataFormFBO2D(unsigned int level, unsigned int xoffset, unsigned int yoffset, unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

		virtual void Bind(int unit = 0);
		virtual void Unbind();
		virtual void DisplayTexture(bool invert = true);
		virtual void ClearTexture(const glm::vec4& color);

		// 两个函数只能在Generate之前使用
		inline void SetTextureSettings(TextureSettings settings) { m_TextureSettings = settings; }
		inline void SetTextureFormat(int format) { m_TextureSettings.TextureFormat = format; }

		// Getters
		virtual inline unsigned int GetTextureId() const override { return m_RendererID; }
		virtual inline unsigned int GetTextureTarget() const override { return m_TextureTarget; }
		virtual inline bool IsGenerated() const override { return m_RendererID != 0; }
		virtual inline unsigned int GetWidth() const override { return m_Width; }
		virtual inline unsigned int GetHeight() const override { return m_Height; }
		virtual inline TextureSettings& GetTextureSettings() override { return m_TextureSettings; }

	private:
		// 应用Texture的设置
		void ApplyTextureSettings();
	private:
		int m_TextureTarget;
		unsigned int m_RendererID;
		unsigned int m_Width, m_Height;

		TextureSettings m_TextureSettings;
	};
}