#pragma once
#include "EMT/Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace EMT {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFcn& callback) override { m_Data.EventCallback = callback; }
		inline void SetVSync(bool enabled) override;
		inline bool isVSync() const override { return m_Data.VSync; }

		virtual void* GetNativeWindow() const override { return (void*)m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		struct WindowData {
			unsigned int Width;
			unsigned int Height;
			std::string Title;
			bool VSync;
			EventCallbackFcn EventCallback;
		};
		WindowData m_Data;
	};
}