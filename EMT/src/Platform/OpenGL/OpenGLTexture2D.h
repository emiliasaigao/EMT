#pragma once
#include "EMT/Renderer/Texture/Texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"


namespace EMT {
	struct OpenGLTextureSettings
	{
		// Texture format
		GLenum TextureFormat = GL_NONE; // If set to GL_NONE, the data format will be used

		/* isSRGB will let the loader know that the texture needs to be "linearlized" before it is sampled in the shaders (i.e the texture is in a non liner space)
		.Note: If you generate your own data and it is already in linear space (like light probes), be careful */
		bool IsSRGB = false;

		// Texture wrapping options
		GLenum TextureWrapSMode = GL_REPEAT;
		GLenum TextureWrapTMode = GL_REPEAT;
		bool HasBorder = false;
		glm::vec4 BorderColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); //default border used for shadow map

		// Texture filtering options
		GLenum TextureMinificationFilterMode = GL_LINEAR_MIPMAP_LINEAR; // Filtering mode when the texture moves further away and multiple texels map to one pixel (trilinear for best quality)
		GLenum TextureMagnificationFilterMode = GL_LINEAR; // Filtering mode when the texture gets closer and multiple pixels map to a single texel (Never needs to be more than bilinear because that is as accurate as it gets in this situation)

		// Mip options
		bool HasMips = true;
		int MipBias = 0; // positive means blurrier texture selected, negative means sharper texture which can show texture aliasing

		// Record data type for framebuffer resizing
		GLenum dataFormat = GL_NONE;
		GLenum dataType = GL_NONE;

		int ChannelNum = 0;
	};



	//class OpenGLTexture2D : public Texture2D {
	//public:
	//	OpenGLTexture2D():m_Height(0),m_Width(0),m_RendererID(0) {}
	//	OpenGLTexture2D(const std::string& path);
	//	~OpenGLTexture2D();
	//	virtual unsigned int GetHeight() const override { return m_Height; }
	//	virtual unsigned int GetWidth() const override { return m_Width; }
	//	virtual void Bind(unsigned int slot) const override;
	//private:
	//	unsigned int m_Height, m_Width;
	//	unsigned int m_RendererID;
	//	std::string m_Path;
	//
	//};
}