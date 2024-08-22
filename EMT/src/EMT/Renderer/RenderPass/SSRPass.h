#pragma once
#include "RenderPass.h"
#include "EMT/EMTEnum.h"
#include "EMT/Renderer/Renderer.h"

namespace EMT {
	class SSRPass : public RenderPass {
	public:
		SSRPass(const Ref<Scene>& scene);
		~SSRPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	private:
		// 简单的将模型渲染到屏幕上的shader
		Ref<Shader> mSSRModelStencilShader;
	};
}