#pragma once
#include "EMT/Renderer/FrameBuffer.h"
#include "glad/glad.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

namespace EMT {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(int width, int height, bool isMultiSample);
		~OpenGLFrameBuffer();

		virtual void AddColorTexture(const TextureSettings& textureSettings, int dataFormat, int dataType, int attachType) override;
		virtual void AddDepthStencilTexture(const TextureSettings& textureSettings, int dataFormat, int dataType, int attachType) override;
		virtual void AddDepthStencilRBO(int rboFormat, int attachType) override;
		virtual void SetUpFrameBuffer() override;

		virtual void ResizeFrameBuffer(int width, int height) override;

		virtual void Clear() override;
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void SaverRenderFrame(const std::string& savePath) override;


		virtual inline int GetWidth() override { return m_Width; }
		virtual inline int GetHeight() override { return m_Height; }

		virtual unsigned int GetFramebuffer() override { return m_FBORendererID; }
		virtual unsigned int GetDepthStencilRBO() override { return m_RBORendererID; }
		virtual Ref<Texture> GetColourTexture() override { return m_ColorTexture; }
		virtual Ref<Texture> GetDepthStencilTexture() override { return m_DepthStencilTexture; }

	private:
		unsigned int m_FBORendererID;
		int m_Width, m_Height;
		bool m_IsMultiSample;

		// default framebuffer's attachment
		Ref<Texture> m_ColorTexture;
		Ref<Texture> m_DepthStencilTexture;

		int m_RBOFormat;
		unsigned int m_RBORendererID;
		bool m_IsUseRBO;
	};
}