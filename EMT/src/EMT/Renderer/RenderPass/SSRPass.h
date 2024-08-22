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
		// �򵥵Ľ�ģ����Ⱦ����Ļ�ϵ�shader
		Ref<Shader> mSSRModelStencilShader;
	};
}