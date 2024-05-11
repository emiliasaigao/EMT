#pragma once
#include "EMT/Core.h"


// ������������ʵ��Event��������鴿�麯������Ĵ�����
// '#'��һ��Ԥ�������������������ת��Ϊ�ַ�������ֵ
// '##'����һ��Ԥ�������������������Ԥ����׶ν��б������
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; } \
								virtual EventType GetEventType() const override { return GetStaticType(); } \
								virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

namespace EMT {
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication	 = BIT(0),
		EventCategoryInput			 = BIT(1),
		EventCategoryKeyboard		 = BIT(2),
		EventCategoryMouse			 = BIT(3),
		EventCategoryMouseButton	 = BIT(4)
	};

	class EMT_API Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); };

		inline bool isInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_isHandled = false;
	};

	class EventDispatcher {
		// ����T���÷���bool�ķº���
		template <class T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event) : m_Event(event) {}

		template <class T>
		bool DisPacth(EventFn<T> func) {
			if (T::GetStaticType() == m_Event.GetEventType()) {
				m_Event.m_isHandled = func(*(T*)(&m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		os << e.ToString();
		return os;
	}
}