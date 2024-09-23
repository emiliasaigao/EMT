#pragma once
#include "EMT/Renderer/GBuffer.h"
#include "EMT/Renderer/Scene.h"
#include "EMT/Renderer/Shader.h"

namespace EMT {
	struct ShadowmapPassOutput {
		esgstl::vector<glm::mat4> lightSpaceMatrices;
		Ref<FrameBuffer> fbo;
		esgstl::vector<std::pair<float, float>> frustum;
	};

	struct GeometryPassOutput {
		Ref<GBuffer> gbuffer;
	};

	struct IrradianceMapPassOutput {
		Ref<Cubemap> irradianceCubemap;
		Ref<FrameBuffer> fbo;
	};

	struct LightPreFilterPassOutput {
		Ref<Cubemap> lPreFilterCubemap;
		Ref<FrameBuffer> fbo;
	};

	struct LutPassOutput {
		Ref<FrameBuffer> fbo;
	};

	struct EavgPassOutput {
		Ref<FrameBuffer> fbo;
	};

	struct LightingPassOutput {
		Ref<FrameBuffer> fbo;
	};

	struct PostProcessOutput {
		Ref<FrameBuffer> fbo;
	};

	struct SSRPassOutput {
		Ref<FrameBuffer> fbo;
	};

	struct SSAOPassOutput {
		Ref<FrameBuffer> fbo;
	};

	struct SSAOBlurPassOutput {
		Ref<FrameBuffer> fbo;
	};

	struct PassThroughOutput {
		Ref<FrameBuffer> fbo;
	};

	struct RenderPassContext {
		int windowWidth, windowHeight;
		ShadowmapPassOutput shadowOutput;
		GeometryPassOutput geometryOutput;
		IrradianceMapPassOutput irradianceMapOutput;
		LightPreFilterPassOutput lPrefilterOutput;
		LutPassOutput lutOutput;
		EavgPassOutput eavgOutput;
		LightingPassOutput lightOutput;
		SSAOPassOutput ssaoOutput;
		SSAOBlurPassOutput ssaoBlurOutput;
		PostProcessOutput postprocOutput;
		SSRPassOutput ssrOutput;
		PassThroughOutput passThroughOutput;
		int shadowType = 0;
		float PCSSLightSize = 15;
		bool useSSAO = true;
		bool useSSR = false;
		float ssrThickness = 1.2;
		float ssrEffect = 1.0;
		float SSAOEffect = 1.0;
		float SSAORadius = 2.0;
		int SSAOKernelSize = 32;
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
		static RenderPassContext s_Context;
	};
}