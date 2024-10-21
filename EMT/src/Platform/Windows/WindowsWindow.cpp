#include "emtpch.h"
#include "WindowsWindow.h"
#include "EMT/Event/ApplicationEvent.h"
#include "EMT/Event/KeyEvent.h"
#include "EMT/Event/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace EMT {
	static bool s_GLFWInitialized = false;
	
	static void GLFWErrorCallBack(int error, const char* description) {
		EMT_CORE_ERROR("GLFWERROR ({0}) : {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Title = props.Title;
		EMT_CORE_INFO("创建窗口 {0} ({1},{2})", props.Title, props.Width, props.Height);
		if (!s_GLFWInitialized) {
			int success = glfwInit();
			EMT_CORE_ASSERT(success, "GLFW初始化失败！");
			glfwSetErrorCallback(GLFWErrorCallBack);
			s_GLFWInitialized = success;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // 设置主版本号为4
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  // 设置次版本号为6
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 使用核心配置文件s
		if (m_HideCursor)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// 设置回调函数，即发生各种事件时，会调用指定类型的函数，用户实现这个函数可以在里面执行自己的逻辑
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
			data.Width = width;
			data.Height = height;
			WindowResizeEvent e(width, height);
			// data里的eventCallback已经在Application里设置过了，这里是调用，具体做了啥要看OnEvent函数
			data.EventCallback(e);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
			WindowCloseEvent e;
			data.EventCallback(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.EventCallback(e);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
			KeyTypedEvent e(keycode);
			data.EventCallback(e);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS :
				{
					MouseButtonPressedEvent e(button);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.EventCallback(e);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOff, double yOff) {
			WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
			MouseScrolledEvent e((float)xOff, (float)yOff);
			data.EventCallback(e);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));
			MouseMovedEvent e((float)xPos, (float)yPos);
			data.EventCallback(e);
		});
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) 
			glfwSwapInterval(1);
		else 
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}
	
	inline void WindowsWindow::HideCursor() {
		m_HideCursor = !m_HideCursor;
		if (m_HideCursor) 
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else 
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}