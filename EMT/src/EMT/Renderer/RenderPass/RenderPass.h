#pragma once
#include "EMT/Renderer/GBuffer.h"
#include "EMT/Renderer/Scene.h"
#include "EMT/Renderer/Shader.h"

namespace EMT {
	struct ShadowmapPassOutput {
		glm::mat4 lightSpaceMatrix;
		Ref<FrameBuffer> fbo;
	};

	struct GeometryPassOutput {
		Ref<GBuffer> gbuffer;
	};

	struct LightingPassOutput {
		Ref<FrameBuffer> fbo;
	};

	struct PostProcessOutput {
		Ref<FrameBuffer> fbo;
	};

	struct SSRPassOutput {
		Ref<FrameBuffer> fbO;
	};

	struct PassThroughOutput {
		Ref<FrameBuffer> fbo;
	};

	struct RenderPassContext {
		int windowWidth, windowHeight;
		ShadowmapPassOutput shadowOutput;
		GeometryPassOutput geometryOutput;
		LightingPassOutput lightOutput;
		PostProcessOutput postOutput;
		SSRPassOutput ssrOutput;
		PassThroughOutput passThroughOutput;
	};

	class RenderPass {
	friend class RenderPipeLine;
	public:
		RenderPass(const Ref<Scene>& scene)
			:m_Scene(scene){}
		virtual ~RenderPass() {}

		virtual void OnWindowResize() {}
		virtual void Draw() = 0;

	protected:
		Ref<Shader> m_Shader;
		Ref<Scene> m_Scene;
		static RenderPassContext m_Context;
	};
}