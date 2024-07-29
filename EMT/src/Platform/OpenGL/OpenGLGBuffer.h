#pragma once
#include "EMT/Renderer/GBuffer.h"
#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"

namespace EMT {
	class OpenGLGBuffer : public GBuffer, public OpenGLFrameBuffer {
	public:
		OpenGLGBuffer(int width, int height);
		~OpenGLGBuffer() {}

		virtual void AddColorTexture(const TextureSettings& textureSettings, int dataFormat, int dataType, int attachType) override {
			OpenGLFrameBuffer::AddColorTexture(textureSettings, dataFormat, dataType, attachType);
		}
		virtual void AddDepthStencilTexture(const TextureSettings& textureSettings, int dataFormat, int dataType, int attachType) override {
			OpenGLFrameBuffer::AddDepthStencilTexture(textureSettings, dataFormat, dataType, attachType);
		}
		virtual void AddDepthStencilRBO(int rboFormat, int attachType) override {
			OpenGLFrameBuffer::AddDepthStencilRBO(rboFormat, attachType);
		}
		virtual void SetUpFrameBuffer() override {
			OpenGLFrameBuffer::SetUpFrameBuffer();
		}
		virtual void Bind() override {
			OpenGLFrameBuffer::Bind();
		}
		virtual void UnBind() override {
			OpenGLFrameBuffer::UnBind();
		}
		virtual void SaverRenderFrame(const std::string& savePath) override {
			OpenGLFrameBuffer::SaverRenderFrame(savePath);
		}
		virtual inline int GetWidth() override { return m_Width; }
		virtual inline int GetHeight() override { return m_Height; }

		virtual unsigned int GetFramebuffer() override { return m_FBORendererID; }
		virtual unsigned int GetDepthStencilRBO() override { return m_RBORendererID; }
		virtual Ref<Texture> GetColourTexture() override { return m_ColorTexture; }
		virtual Ref<Texture> GetDepthStencilTexture() override { return m_DepthStencilTexture; }


		virtual void Clear() override;
		virtual void ResizeFrameBuffer(int width, int height) override;


		virtual inline Ref<Texture> GetColorTextureByIndex(int index) override { return mRenderTargets[index]; }
		virtual inline std::vector<Ref<Texture>>& GetColorTextures() override { return mRenderTargets; };
	private:
		void Init();
	
	private:
		int mRenderTargetsCount;


		// 0 反照率贴图   RGBA8  ->       albedo.r     albedo.g        albedo.b     albedo's alpha
		// 1 法线贴图     RGB32F ->       normal.x     normal.y        normal.z
		// 2 世界坐标贴图 RGB32F ->       pos.x		  pos.y		      pos.z
		// 3 混合贴图     RGB8  ->        roughness    metallic        ambientOcclusion
		std::vector<Ref<Texture>> mRenderTargets;
	};
}