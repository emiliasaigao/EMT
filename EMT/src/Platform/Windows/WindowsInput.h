#pragma once
#include "EMT/Input.h"

namespace EMT {
	class WindowsInput : public Input {
		// Í¨¹ý Input ¼Ì³Ð
	protected:
		virtual bool isKeyPressedImpl(int key) override;
		virtual bool isMouseBottomPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
	};
}