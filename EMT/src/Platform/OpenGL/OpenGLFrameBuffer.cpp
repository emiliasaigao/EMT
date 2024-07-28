#include "emtpch.h"
#include "OpenGLFrameBuffer.h"

namespace EMT {
	OpenGLFrameBuffer::OpenGLFrameBuffer(int width, int height, bool isMultiSample) :
		m_Width(width), m_Height(height), m_IsMultiSample(isMultiSample) {
		
		m_IsUseRBO = false;
		glGenFramebuffers(1, &m_FBORendererID);

	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &m_FBORendererID);
		if (m_IsUseRBO)
			glDeleteRenderbuffers(1, &m_RBORendererID);
	}

	void OpenGLFrameBuffer::AddColorTexture(const TextureSettings& textureSettings, int dataFormat, int dataType, int attachType) {
		Bind();
		m_ColorTexture = Texture::Create(textureSettings);
		m_ColorTexture->Generate2DTexture(m_Width, m_Height, dataFormat, dataType);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachType, GL_TEXTURE_2D, m_ColorTexture->GetTextureId(), 0);
		UnBind();
	}

	void OpenGLFrameBuffer::AddDepthStencilTexture(const TextureSettings& textureSettings, int dataFormat, int dataType, int attachType) {
		Bind();
		m_DepthStencilTexture =Texture::Create(textureSettings);
		m_DepthStencilTexture->Generate2DTexture(m_Width, m_Height, dataFormat, dataType);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachType, GL_TEXTURE_2D, m_DepthStencilTexture->GetTextureId(), 0);
		UnBind();
	}


	/// <summary>
	/// 如果需要深度/模板缓冲的话
	/// </summary>
	/// <param name="rboFormat">rbo格式，比如GL_DEPTH24_STENCIL8</param>
	/// <param name="attachType">附件类型，比如GL_DEPTH_STENCIL_ATTACHMENT</param>
	void OpenGLFrameBuffer::AddDepthStencilRBO(int rboFormat, int attachType) {
		m_RBOFormat = rboFormat;
		m_IsUseRBO = true;
		Bind();

		glGenRenderbuffers(1, &m_RBORendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBORendererID);
		glRenderbufferStorage(GL_RENDERBUFFER, rboFormat, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachType, GL_RENDERBUFFER, m_RBORendererID);

		UnBind();
	}

	void OpenGLFrameBuffer::SetUpFrameBuffer() {
		Bind();
		if (!m_ColorTexture->IsGenerated()) {
			// 明确指定这个FBO不需要Color输出
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		// 检查帧缓冲是否可用
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			EMT_CORE_ERROR("FRAMEBUFFER 不完整!");
			return;
		}
		UnBind();
	}

	void OpenGLFrameBuffer::ResizeFrameBuffer(int width, int height) {
		m_Width = width;
		m_Height = height;

		if (m_ColorTexture) {
			m_ColorTexture->Bind(0);
			const TextureSettings& settings = m_ColorTexture->GetTextureSettings();
			glTexImage2D(m_ColorTexture->GetTextureTarget(), 0, settings.TextureFormat, width, height, 0, settings.dataFormat, settings.dataType, nullptr);
			m_ColorTexture->Unbind();
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

	void OpenGLFrameBuffer::Clear() {
		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLFrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBORendererID);
	}

	void OpenGLFrameBuffer::UnBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::SaverRenderFrame(const std::string& savePath) {
		int width = m_Width;
		int height = m_Height;
		Bind();

		unsigned char* data = new unsigned char[width * height * 3];
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);


		unsigned char* flippedData = new unsigned char[width * height * 3];
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int index1 = (i * width + j) * 3;
				int index2 = ((height - 1 - i) * width + j) * 3;
				flippedData[index1] = data[index2];
				flippedData[index1 + 1] = data[index2 + 1];
				flippedData[index1 + 2] = data[index2 + 2];
			}
		}

		stbi_write_png(std::string(savePath + ".jpg").c_str(), width, height,
			3, flippedData, width * 3);

		delete[] data;
		delete[] flippedData;
	}

}
