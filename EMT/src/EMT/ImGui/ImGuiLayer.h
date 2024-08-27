#pragma once

#include "EMT/Layer.h"
#include "EMT/Event/ApplicationEvent.h"
#include "EMT/Event/KeyEvent.h"
#include "EMT/Event/MouseEvent.h"
#include "EMT/Renderer/Scene.h"
#include "EMT/Renderer/RenderPass/RenderPipeline.h"

namespace EMT {
	class EMT_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer(const Ref<Scene>& scene, const Ref<RenderPipeLine>& pipeLine);
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
		
	private:
		float m_time = 0.f;
		Ref<Scene> m_Scene;
		Ref<RenderPipeLine> m_PipeLine;
		esgstl::vector<Ref<Skybox>> m_SkyBoxes;
		int mCurSkyBoxIdx = 0;
		int mLastSkyBoxIdx = 0;
	};
}