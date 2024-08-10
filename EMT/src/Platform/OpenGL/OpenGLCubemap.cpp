#include "emtpch.h"
#include "OpenGLCubemap.h"

namespace EMT {
	OpenGLCubemap::OpenGLCubemap(const CubemapSettings& settings) :
		m_RendererID(0), m_Width(0), m_Height(0),
		m_GeneratedFacesNum(0), m_CubeMapSettings(settings), m_CubeMapFaces(6) {


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
		TextureSettings settings;
		/*settings.TextureFormat = m_CubeMapSettings.TextureFormat;
		settings.HasMips = m_CubeMapSettings.HasMips;
		m_CubeMapFaces[m_GeneratedFacesNum] = Texture::Create(settings);
		m_CubeMapFaces[m_GeneratedFacesNum]->Generate2DTexture(width, height, dataFormat, pixelDataType, data);*/
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_RGB16F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		m_CubeMapFaces[m_GeneratedFacesNum] = Texture::Create(colorTextureSettings);
		m_CubeMapFaces[m_GeneratedFacesNum]->Generate2DTexture(64, 64, EMT_RGB, EMT_FLOAT);
		++m_GeneratedFacesNum;

		if (m_GeneratedFacesNum >= 6)
			ApplyCubemapSettings();


		UnBind();
	}

	void OpenGLCubemap::DisplayTexture(bool invert) {
		for (int i = 0; i < 6; ++i) {
			auto face = m_CubeMapFaces[i];
			if (face) {
				ImGui::Text("size = %d x %d", face->GetWidth(), face->GetHeight());
				if (invert)
					ImGui::Image((void*)(intptr_t)face->GetTextureId(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
				else ImGui::Image((void*)(intptr_t)face->GetTextureId(), ImVec2(face->GetWidth(), face->GetHeight()));
			}
		}
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