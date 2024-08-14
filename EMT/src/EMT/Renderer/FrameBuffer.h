#pragma once
#include "Texture/Texture.h"

namespace EMT {
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {}

		virtual void AddColorTexture(const TextureSettings& textureSettings, int dataFormat, int dataType, int attachType) = 0;
		virtual void SetColorTexture(int attachType, unsigned int textureType, unsigned int textureId, int miplevel) = 0;
		virtual void AddDepthStencilTexture(const TextureSettings& textureSettings, int dataFormat, int dataType, int attachType) = 0;
		virtual void AddDepthStencilTextureArray(const TextureSettings& textureSettings, int dataFormat, int dataType, int attachType) = 0;
		virtual void AddDepthStencilRBO(int rboFormat, int attachType) = 0;
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
		virtual Ref<Texture> GetColorTexture() = 0;
		virtual Ref<Texture> GetDepthStencilTexture() = 0;

		static Ref<FrameBuffer> Create(int width, int height, bool isMultiSample = false);
	};
}