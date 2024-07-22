#pragma once
#include "EMT/Renderer/Texture.h"

namespace EMT {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D():m_Height(0),m_Width(0),m_RendererID(0) {}
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();
		virtual unsigned int GetHeight() const override { return m_Height; }
		virtual unsigned int GetWidth() const override { return m_Width; }
		virtual void Bind(unsigned int slot) const override;
	private:
		unsigned int m_Height, m_Width;
		unsigned int m_RendererID;
		std::string m_Path;

	};
}