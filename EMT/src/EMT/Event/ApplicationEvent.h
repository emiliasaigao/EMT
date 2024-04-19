#pragma once
#include "Event.h"

#include <sstream>

namespace EMT {
	// WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	// AppTick, AppUpdate, AppRender,
	class EMT_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class EMT_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}
		
		inline unsigned int getWidth() const { return m_Width; }
		inline unsigned int getHeight() const { return m_Height; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent:" << m_Width << "," << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		unsigned int m_Width, m_Height;
	};

	class AppTickEvent : public Event {
	public:
		AppTickEvent() {}
		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}
		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}
		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

}
