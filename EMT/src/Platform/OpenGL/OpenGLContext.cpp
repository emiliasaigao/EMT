#include "emtpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace EMT {
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_Window(window)
	{
		EMT_CORE_ASSERT(m_Window, "m_Window�ǿ�ָ�룡")
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EMT_CORE_ASSERT(status, "Glad ��ʼ��ʧ�ܣ�");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}