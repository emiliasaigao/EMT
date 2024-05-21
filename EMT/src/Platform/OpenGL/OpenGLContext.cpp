#include "emtpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace EMT {
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_Window(window)
	{
		EMT_CORE_ASSERT(m_Window, "m_Window是空指针！")
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EMT_CORE_ASSERT(status, "Glad 初始化失败！");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}