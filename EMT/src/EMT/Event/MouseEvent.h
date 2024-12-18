#pragma once

#include "Event.h"


namespace EMT {
	class EMT_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float xOffset, float yOffset) : m_MouseX(xOffset), m_MouseY(yOffset) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << "," << m_MouseY;
			return ss.str().data();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

	private:
		float m_MouseX, m_MouseY;
	};

	class EMT_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << "," << m_YOffset;
			return ss.str().data();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

	private:
		float m_XOffset, m_YOffset;
	};

	class EMT_API MouseButtonEvent : public Event {
	public:
		inline int GetMouseButtonCode() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton);

	protected:
		MouseButtonEvent(int button) : m_Button(button) {}
		int m_Button;
	};


	class EMT_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent:" << m_Button;
			return ss.str().data();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);

	};

	class EMT_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}


		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent:" << m_Button;
			return ss.str().data();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}