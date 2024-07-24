#include "emtpch.h"
#include "Texture.h"
#include "EMT/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace EMT {

	Texture::Texture(const TextureSettings& settings /*= TextureSettings()*/) :
		m_RendererID(0), m_TextureTarget(0), m_Width(0), m_Height(0), m_TextureSettings(settings)
	{
	}

	Texture::~Texture()
	{
		if (m_RendererID != 0)
			glDeleteTextures(1, &m_RendererID);
	}

	void Texture::Generate2DTexture(unsigned int width, unsigned int height, GLenum dataFormat, GLenum pixelDataType, const void* data)
	{
		m_TextureTarget = GL_TEXTURE_2D;
		m_Width = width;
		m_Height = height;

		// 如果纹理格式为GL_NONE，将纹理格式设置为数据格式
		if (m_TextureSettings.TextureFormat == GL_NONE)
			m_TextureSettings.TextureFormat = dataFormat;

		// 检查纹理是否为SRGB（非线性空间），如果是，更改纹理格式并让gpu进行伽玛校正
		if (m_TextureSettings.IsSRGB)
		{
			switch (dataFormat)
			{
			case GL_RGB: m_TextureSettings.TextureFormat = GL_SRGB; break;
			case GL_RGBA: m_TextureSettings.TextureFormat = GL_SRGB_ALPHA; break;
			}
		}

		// 生成纹理，并绑定数据
		glGenTextures(1, &m_RendererID);
		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, m_TextureSettings.TextureFormat, width, height, 0, dataFormat, pixelDataType, data);

		// 记录数据格式和数据类型
		m_TextureSettings.dataFormat = dataFormat;
		m_TextureSettings.dataType = pixelDataType;

		// 应用这些设置
		ApplyTextureSettings();
		Unbind();
	}

	void Texture::Generate2DMultisampleTexture(unsigned int width, unsigned int height)
	{
		// 多采样纹理不支持mipmap、过滤和环绕选项
		m_TextureTarget = GL_TEXTURE_2D_MULTISAMPLE;
		m_Width = width;
		m_Height = height;

		glGenTextures(1, &m_RendererID);
		Bind();
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLE_AMOUNT, m_TextureSettings.TextureFormat, m_Width, m_Height, GL_TRUE);
		Unbind();
	}

	void Texture::GenerateMips()
	{
		m_TextureSettings.HasMips = true;
		if (IsGenerated())
		{
			Bind();
			glGenerateMipmap(m_TextureTarget);
			Unbind();
		}
	}

	void Texture::Bind(int unit /*= 0*/)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(m_TextureTarget, m_RendererID);
	}


	void Texture::Unbind()
	{
		glBindTexture(m_TextureTarget, 0);
	}

	void Texture::ApplyTextureSettings()
	{
		// 设置纹理环绕方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TextureSettings.TextureWrapSMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TextureSettings.TextureWrapTMode);
		if (m_TextureSettings.HasBorder)
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &m_TextureSettings.BorderColour[0]);


		// 设置纹理过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TextureSettings.TextureMinificationFilterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TextureSettings.TextureMagnificationFilterMode);

		// 生成mipmap
		if (m_TextureSettings.HasMips)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, m_TextureSettings.MipBias);
		}
	}
	void Texture::DisplayTexture(bool invert /*= true*/) {
		ImGui::Text("size = %d x %d", m_Width, m_Height);
		if (invert)
			ImGui::Image((void*)(intptr_t)m_RendererID, ImVec2(m_Width, m_Height), ImVec2(0, 1), ImVec2(1, 0));
		else ImGui::Image((void*)(intptr_t)m_RendererID, ImVec2(m_Width, m_Height));
	}

		//Ref<Texture2D> Texture2D::Create(const std::string& path)
	//{
	//	switch (Renderer::GetAPI())
	//	{
	//		case RendererAPI::API::None:		EMT_CORE_ASSERT(false, "现在还不支持RenderAPI::None"); return nullptr;
	//		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(path);
	//	}
	//	EMT_CORE_ASSERT(false, "尚未选择RenderAPI");
	//	return nullptr;
	//}
}