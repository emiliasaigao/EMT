#pragma once
#include "emtpch.h"

#include "Core.h"
#include "EMT/Event/Event.h"

namespace EMT {
	struct WindowProps {
		unsigned int Width;
		unsigned int Height;
		std::string Title;
		WindowProps(unsigned int w = 1024, unsigned int h = 720, const std::string& title = "EMT Engine")
			: Width(w), Height(h), Title(title) {}
	};

	class EMT_API Window {
	public:
		using EventCallbackFcn = std::function<void(Event&)>;
		
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// 获取/修改窗口属性
		virtual void SetEventCallback(const EventCallbackFcn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0; // 设置垂直同步
		virtual bool isVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}