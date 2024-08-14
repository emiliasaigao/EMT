#include "emtpch.h"
#include "OpenGLTexture.h"

namespace EMT {
	OpenGLTexture::OpenGLTexture(const TextureSettings& settings) :
		m_RendererID(0), m_TextureTarget(0), m_Width(0), m_Height(0), m_TextureSettings(settings) {
	}

	OpenGLTexture::~OpenGLTexture() {
		if (m_RendererID != 0)
			glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture::Generate2DTexture(unsigned int width, unsigned int height, int dataFormat, int pixelDataType, const void* data) {
		m_TextureTarget = EMT_TEXTURE_2D;
		m_Width = width;
		m_Height = height;

		// 如果纹理格式为EMT_NONE，将纹理格式设置为数据格式
		if (m_TextureSettings.TextureFormat == EMT_NONE)
			m_TextureSettings.TextureFormat = dataFormat;

		// 检查纹理是否为SRGB（非线性空间），如果是，更改纹理格式并让gpu进行伽玛校正
		if (m_TextureSettings.IsSRGB) {
			switch (dataFormat)
			{
				case EMT_RGB: m_TextureSettings.TextureFormat = GL_SRGB; break;
				case EMT_RGBA: m_TextureSettings.TextureFormat = GL_SRGB_ALPHA; break;
			}
		}

		// 生成纹理，并绑定数据
		glGenTextures(1, &m_RendererID);
		Bind();
		glTexImage2D(EMT_TEXTURE_2D, 0, 
			m_TextureSettings.TextureFormat, 
			width, height, 0, dataFormat, 
			pixelDataType, data);

		// 记录数据格式和数据类型
		m_TextureSettings.dataFormat = dataFormat;
		m_TextureSettings.dataType = pixelDataType;

		// 应用这些设置
		ApplyTextureSettings();
		Unbind();
	}

	void OpenGLTexture::Generate2DMultisampleTexture(unsigned int width, unsigned int height) {
		// 多采样纹理不支持mipmap、过滤和环绕选项
		m_TextureTarget = GL_TEXTURE_2D_MULTISAMPLE;
		m_Width = width;
		m_Height = height;

		glGenTextures(1, &m_RendererID);
		Bind();
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLE_AMOUNT, 
			m_TextureSettings.TextureFormat, m_Width, m_Height, GL_TRUE);
		Unbind();
	}

	void OpenGLTexture::Genarate2DTextureArray(unsigned int width, unsigned int height, int dataFormat, int pixelDataType, const void* data) {
		m_TextureTarget = GL_TEXTURE_2D_ARRAY;
		m_Width = width;
		m_Height = height;

		// 如果纹理格式为EMT_NONE，将纹理格式设置为数据格式
		if (m_TextureSettings.TextureFormat == EMT_NONE)
			m_TextureSettings.TextureFormat = dataFormat;

		// 检查纹理是否为SRGB（非线性空间），如果是，更改纹理格式并让gpu进行伽玛校正
		if (m_TextureSettings.IsSRGB) {
			switch (dataFormat)
			{
				case EMT_RGB: m_TextureSettings.TextureFormat = GL_SRGB; break;
				case EMT_RGBA: m_TextureSettings.TextureFormat = GL_SRGB_ALPHA; break;
			}
		}

		// 生成纹理，并绑定数据
		glGenTextures(1, &m_RendererID);
		Bind();
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0,
			m_TextureSettings.TextureFormat,
			width, height, 
			EMT::CSCADED_SIZE + 1, 0, dataFormat,
			pixelDataType, data);

		// 记录数据格式和数据类型
		m_TextureSettings.dataFormat = dataFormat;
		m_TextureSettings.dataType = pixelDataType;

		// 应用这些设置
		ApplyTextureSettings();
		Unbind();
	}

	void OpenGLTexture::GenerateMips() {
		m_TextureSettings.HasMips = true;
		if (IsGenerated()) {
			Bind();
			glGenerateMipmap(m_TextureTarget);
			Unbind();
		}
	}

	void OpenGLTexture::CopyDataFormFBO2D(unsigned int level, unsigned int xoffset, unsigned int yoffset, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		Bind();
		glCopyTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, x, y, width, height);
		Unbind();
	}

	void OpenGLTexture::Bind(int unit /*= 0*/) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(m_TextureTarget, m_RendererID);
	}


	void OpenGLTexture::Unbind() {
		glBindTexture(m_TextureTarget, 0);
	}

	void OpenGLTexture::ApplyTextureSettings() {
		// 设置纹理环绕方式
		glTexParameteri(m_TextureTarget, EMT_TEXTURE_WRAP_S, m_TextureSettings.TextureWrapSMode);
		glTexParameteri(m_TextureTarget, EMT_TEXTURE_WRAP_T, m_TextureSettings.TextureWrapTMode);
		if (m_TextureSettings.HasBorder)
			glTexParameterfv(m_TextureTarget, GL_TEXTURE_BORDER_COLOR, &m_TextureSettings.BorderColour[0]);


		// 设置纹理过滤方式
		glTexParameteri(m_TextureTarget, EMT_TEXTURE_MIN_FILTER, m_TextureSettings.TextureMinificationFilterMode);
		glTexParameteri(m_TextureTarget, EMT_TEXTURE_MAG_FILTER, m_TextureSettings.TextureMagnificationFilterMode);

		// 生成mipmap
		if (m_TextureSettings.HasMips) {
			glGenerateMipmap(m_TextureTarget);
			glTexParameteri(m_TextureTarget, GL_TEXTURE_LOD_BIAS, m_TextureSettings.MipBias);
		}
	}

	void OpenGLTexture::DisplayTexture(bool invert /*= true*/) {
		ImGui::Text("size = %d x %d", m_Width, m_Height);
		if (invert)
			ImGui::Image((void*)(intptr_t)m_RendererID, ImVec2(m_Width, m_Height), ImVec2(0, 1), ImVec2(1, 0));
		else ImGui::Image((void*)(intptr_t)m_RendererID, ImVec2(512, 512), ImVec2(0, 1), ImVec2(1, 0));
	}
}
