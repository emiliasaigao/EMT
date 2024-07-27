#pragma once
#include "Texture/Texture.h"

namespace EMT {
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {}

		virtual void AddColorTexture(const TextureSettings& textureSettings, GLenum dataFormat, GLenum dataType, GLenum attachType) = 0;
		virtual void AddDepthStencilTexture(const TextureSettings& textureSettings, GLenum dataFormat, GLenum dataType, GLenum attachType) = 0;
		virtual void AddDepthStencilRBO(GLenum rboFormat, GLenum attachType) = 0;
		virtual void SetUpFrameBuffer() = 0;

		// When user changes the window size, we need to resize framebuffer too.
		virtual void ResizeFrameBuffer(int width, int height) = 0;

		virtual void Clear() = 0;
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual void SaverRenderFrame(const std::string& savePath) = 0;
		// getters
		virtual inline int GetWidth() = 0;
		virtual inline int GetHeight() = 0;

		virtual unsigned int GetFramebuffer() = 0;
		virtual unsigned int GetDepthStencilRBO() = 0;
		virtual Ref<Texture> GetColourTexture() = 0;
		virtual Ref<Texture> GetDepthStencilTexture() = 0;

		static Ref<FrameBuffer> Create(int width, int height, bool isMultiSample = false);
	};
}