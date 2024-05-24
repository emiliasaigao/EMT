#include "emtpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLAPI.h"

namespace EMT {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLAPI();
}