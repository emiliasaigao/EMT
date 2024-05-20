#pragma once

#include "EMT/Layer.h"
#include "EMT/Event/ApplicationEvent.h"
#include "EMT/Event/KeyEvent.h"
#include "EMT/Event/MouseEvent.h"

namespace EMT {
	class EMT_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
		
	private:
		float m_time = 0.f;
	};
}