#include "emtpch.h"
#include "OpenGLCubemap.h"

namespace EMT {
	OpenGLCubemap::OpenGLCubemap(const CubemapSettings& settings) :
		m_RendererID(0), m_Width(0), m_Height(0),
		m_GeneratedFacesNum(0), m_CubeMapSettings(settings) {
	}

	OpenGLCubemap::~OpenGLCubemap() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLCubemap::Bind(int unit) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	void OpenGLCubemap::UnBind() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void OpenGLCubemap::GenerateCubemapFace(int face, unsigned int width, unsigned int height, int dataFormat, int pixelDataType, unsigned char* data) {
		if (m_RendererID == 0) {
			glGenTextures(1, &m_RendererID);
			m_Width = width;
			m_Height = height;

			if (m_CubeMapSettings.TextureFormat == GL_NONE)
				m_CubeMapSettings.TextureFormat = dataFormat;

			if (m_CubeMapSettings.IsSRGB == true) {
				switch (m_CubeMapSettings.TextureFormat) {
					case GL_RGB:
						m_CubeMapSettings.TextureFormat = GL_SRGB; break;
					case GL_RGBA:
						m_CubeMapSettings.TextureFormat = GL_SRGB_ALPHA; break;
				}
			}
		}

		Bind();

		glTexImage2D(face, 0, m_CubeMapSettings.TextureFormat, width, height, 0, dataFormat, pixelDataType, data);
		++m_GeneratedFacesNum;

		if (m_GeneratedFacesNum >= 6)
			ApplyCubemapSettings();


		UnBind();
	}

	void OpenGLCubemap::ApplyCubemapSettings() {
		// Texture wrapping
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_CubeMapSettings.TextureWrapSMode);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_CubeMapSettings.TextureWrapTMode);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_CubeMapSettings.TextureWrapRMode);

		// Texture filtering
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_CubeMapSettings.TextureMagnificationFilterMode);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_CubeMapSettings.TextureMinificationFilterMode);

		// Mipmapping
		if (m_CubeMapSettings.HasMips)
		{
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, m_CubeMapSettings.MipBias);
		}
	}

}