#include "emtpch.h"
#include "OpenGLGBuffer.h"

namespace EMT {
	OpenGLGBuffer::OpenGLGBuffer(int width, int height) :
		OpenGLFrameBuffer(width, height) {
		Init();
	}

	void OpenGLGBuffer::Clear() {
		glm::vec4 positionClear(100000.0f); // 无穷远
		glm::vec4 normalClear(0.0f);
		glm::vec4 mixtureClear(0.0f);

		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 1, &normalClear[0]);
		glClearBufferfv(GL_COLOR, 2, &positionClear[0]);
		glClearBufferfv(GL_COLOR, 3, &mixtureClear[0]);
	}

	void OpenGLGBuffer::ResizeFrameBuffer(int width, int height) {
		m_Width = width;
		m_Height = height;

		// resize 4张贴图
		for (int i = 0; i < mRenderTargetsCount; i++) {
			Ref<Texture> current = mRenderTargets[i];
			current->Bind(0);
			const TextureSettings& settings = current->GetTextureSettings();
			glTexImage2D(current->GetTextureTarget(), 0, settings.TextureFormat, width, height, 0, settings.dataFormat, settings.dataType, nullptr);
			current->Unbind();
		}


		if (m_DepthStencilTexture) {
			m_DepthStencilTexture->Bind(0);
			const TextureSettings& settings = m_DepthStencilTexture->GetTextureSettings();
			glTexImage2D(m_DepthStencilTexture->GetTextureTarget(), 0, settings.TextureFormat, width, height, 0, settings.dataFormat, settings.dataType, nullptr);
			m_DepthStencilTexture->Unbind();
		}

		if (m_IsUseRBO) {
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBORendererID);
			glRenderbufferStorage(GL_RENDERBUFFER, m_RBOFormat, m_Width, m_Height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		
	}

	void OpenGLGBuffer::Init() {
		// GBuffer需要RBO，渲染四张贴图均依赖于物体的深度值
		TextureSettings depthStencilTextureSettings;
		depthStencilTextureSettings.TextureFormat = GL_DEPTH24_STENCIL8;
		depthStencilTextureSettings.TextureWrapSMode = GL_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureWrapTMode = GL_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureMagnificationFilterMode = GL_NEAREST;
		depthStencilTextureSettings.TextureMinificationFilterMode = GL_NEAREST;
		depthStencilTextureSettings.HasBorder = true;	// use default (1,1,1) border color
		depthStencilTextureSettings.HasMips = false;
		OpenGLFrameBuffer::AddDepthStencilTexture(depthStencilTextureSettings, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, GL_DEPTH_STENCIL_ATTACHMENT);

		// 生成四张GBuffer纹理
		OpenGLFrameBuffer::Bind();
		mRenderTargetsCount = 4;
		// 0 RGBA8  ->       albedo.r     albedo.g        albedo.b     albedo's alpha
		TextureSettings renderTarget1;
		renderTarget1.TextureFormat = GL_RGBA8;
		renderTarget1.TextureWrapSMode = GL_CLAMP_TO_EDGE;
		renderTarget1.TextureWrapTMode = GL_CLAMP_TO_EDGE;
		renderTarget1.TextureMinificationFilterMode = GL_NEAREST;
		renderTarget1.TextureMagnificationFilterMode = GL_NEAREST;
		renderTarget1.HasMips = false;
		Ref<Texture> albedoTargets = Texture::Create(renderTarget1);
		albedoTargets->Generate2DTexture(m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE);
		mRenderTargets.push_back(albedoTargets);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTargets[0]->GetTextureId(), 0);

		// 1 RGB32F ->       normal.x     normal.y        normal.z
		TextureSettings renderTarget2;
		renderTarget2.TextureFormat = GL_RGB32F;
		renderTarget2.TextureWrapSMode = GL_CLAMP_TO_EDGE;
		renderTarget2.TextureWrapTMode = GL_CLAMP_TO_EDGE;
		renderTarget2.TextureMinificationFilterMode = GL_NEAREST;
		renderTarget2.TextureMagnificationFilterMode = GL_NEAREST;
		renderTarget2.HasMips = false;
		Ref<Texture> normalTargets = Texture::Create(renderTarget2);
		normalTargets->Generate2DTexture(m_Width, m_Height, GL_RGB, GL_FLOAT);
		mRenderTargets.push_back(normalTargets);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mRenderTargets[1]->GetTextureId(), 0);
		// 2 RGB32F ->       pos.x		  pos.y		      pos.z
		TextureSettings renderTarget3;
		renderTarget3.TextureFormat = GL_RGB32F;
		renderTarget3.TextureWrapSMode = GL_CLAMP_TO_EDGE;
		renderTarget3.TextureWrapTMode = GL_CLAMP_TO_EDGE;
		renderTarget3.TextureMinificationFilterMode = GL_NEAREST;
		renderTarget3.TextureMagnificationFilterMode = GL_NEAREST;
		renderTarget3.HasMips = false;
		Ref<Texture> positionTargets = Texture::Create(renderTarget3);
		positionTargets->Generate2DTexture(m_Width, m_Height, GL_RGB, GL_FLOAT);
		mRenderTargets.push_back(positionTargets);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mRenderTargets[2]->GetTextureId(), 0);
		// 3 RGB8  ->        roughness    metallic        ambientOcclusion
		TextureSettings renderTarget4;
		renderTarget4.TextureFormat = GL_RGB8;
		renderTarget4.TextureWrapSMode = GL_CLAMP_TO_EDGE;
		renderTarget4.TextureWrapTMode = GL_CLAMP_TO_EDGE;
		renderTarget4.TextureMinificationFilterMode = GL_NEAREST;
		renderTarget4.TextureMagnificationFilterMode = GL_NEAREST;
		renderTarget4.HasMips = false;
		Ref<Texture> mixtureTargets = Texture::Create(renderTarget4);
		mixtureTargets->Generate2DTexture(m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE);
		mRenderTargets.push_back(mixtureTargets);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mRenderTargets[3]->GetTextureId(), 0);

		// 绑定四个framebuffer的颜色输出
		unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			EMT_CORE_ERROR("FRAMEBUFFER 不完整!");
			return;
		}

		OpenGLFrameBuffer::UnBind();
	}
}