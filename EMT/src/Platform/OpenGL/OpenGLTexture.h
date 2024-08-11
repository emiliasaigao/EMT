#pragma once
#include "EMT/Renderer/Texture/Texture.h"
#include "glad/glad.h"

namespace EMT {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const TextureSettings& settings);
		~OpenGLTexture();

		// ����texture
		void Generate2DTexture(unsigned int width, unsigned int height, int dataFormat, int pixelDataType = EMT_UNSIGNED_BYTE, const void* data = nullptr);
		void Generate2DMultisampleTexture(unsigned int width, unsigned int height);
		// ��������MipMap��ֻ���ڱ�Generate֮������
		void GenerateMips();
		void CopyDataFormFBO2D(unsigned int level, unsigned int xoffset, unsigned int yoffset, unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

		void Bind(int unit = 0);
		void Unbind();
		void DisplayTexture(bool invert = true);

		// ��������ֻ����Generate֮ǰʹ��
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
		// Ӧ��Texture������
		void ApplyTextureSettings();
	private:
		int m_TextureTarget;
		unsigned int m_RendererID;
		unsigned int m_Width, m_Height;

		TextureSettings m_TextureSettings;
	};
}