#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class OutlineGenPass : public RenderPass {
	public:
		OutlineGenPass(const Ref<Scene>& scene);
		~OutlineGenPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	private:
		// ��һ�����ʱ��shader
		Ref<Shader> m_DepthGenShader;
	};
}