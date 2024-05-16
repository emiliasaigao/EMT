#pragma once
#include "Core.h"

namespace EMT {
	class EMT_API Input {
	public:
		static inline bool isKeyPressed(int key) { return s_Instance->isKeyPressedImpl(key); }
		static inline bool isMouseButtonPressed(int button) { return s_Instance->isMouseBottomPressedImpl(button); }
		static inline std::pair<float,float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		static inline float GetMouseX() { return s_Instance->GetMousePos().first; }
		static inline float GetMouseY() { return s_Instance->GetMousePos().second; }
	protected:
		virtual bool isKeyPressedImpl(int key) = 0;
		virtual bool isMouseBottomPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
	private:
		static Input* s_Instance;
	};
}