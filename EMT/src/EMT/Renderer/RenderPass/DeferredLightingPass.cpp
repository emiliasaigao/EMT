#include "emtpch.h"
#include "DeferredLightingPass.h"

EMT::DeferredLightingPass::DeferredLightingPass(const Ref<Scene>& scene)
	: RenderPass(scene) {
	m_Shader = Shader::Create("../EMT/assets/shader/deferred/DeferredLighting.vert", "../EMT/assets/shader/deferred/DeferredLighting.frag");

	RenderPass::s_Context.lightOutput.fbo = FrameBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);

	TextureSettings colorTextureSettings;
	colorTextureSettings.TextureFormat = EMT_RGBA32F;
	colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
	colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
	colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
	colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
	colorTextureSettings.HasMips = false;

	TextureSettings depthStencilTextureSettings;
	depthStencilTextureSettings.TextureFormat = EMT_DEPTH24_STENCIL8;
	depthStencilTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_BORDER;
	depthStencilTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_BORDER;
	depthStencilTextureSettings.TextureMagnificationFilterMode = EMT_NEAREST;
	depthStencilTextureSettings.TextureMinificationFilterMode = EMT_NEAREST;
	depthStencilTextureSettings.HasBorder = true;	// use default (1,1,1,1) border color
	depthStencilTextureSettings.HasMips = false;
	RenderPass::s_Context.lightOutput.fbo->AddDepthStencilTexture(depthStencilTextureSettings, EMT_DEPTH_STENCIL, EMT_UNSIGNED_INT_24_8, EMT_DEPTH_STENCIL_ATTACHMENT);
	RenderPass::s_Context.lightOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGBA, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
	RenderPass::s_Context.lightOutput.fbo->SetUpFrameBuffer();
}

void EMT::DeferredLightingPass::Draw() {
	auto mfbo = RenderPass::s_Context.lightOutput.fbo;
	mfbo->Bind();
	mfbo->Clear();

	RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());
	RenderCommand::EnableMultiSample();

	Ref<Camera> camera = m_Scene->GetCamera();
	Ref<LightManager> lightManager = m_Scene->GetLightManager();
	esgstl::vector<glm::mat4>& lightSpaceMatrices = RenderPass::s_Context.shadowOutput.lightSpaceMatrices;
	esgstl::vector<std::pair<float, float>>& frustum = RenderPass::s_Context.shadowOutput.frustum;
	esgstl::vector<float>& frustumSizes = RenderPass::s_Context.shadowOutput.frustumSizes;

	// 将Gbuffer的深度模板缓冲拷贝过来（减少fragment绘制）
	RenderCommand::CopyFBODepthStencil(RenderPass::s_Context.geometryOutput.gbuffer, mfbo);
	
	// shadowPass的结果拿来算阴影
	esgstl::vector<glm::mat4> lightSpaceMatrix = RenderPass::s_Context.shadowOutput.lightSpaceMatrices;
	RenderPass::s_Context.shadowOutput.fbo->GetDepthStencilTexture()->Bind(0);

	RenderCommand::DisableDepthTest();
	RenderCommand::DisableMultiSample();

	
	m_Shader->Bind();

	lightManager->BindCurrentLights(m_Shader);

	m_Shader->setVec3f("viewPos", camera->Position);
	m_Shader->setMat4f("view", camera->getViewMatrix());
	for (int i = 0; i < EMT::CSCADED_SIZE; ++i) {
		m_Shader->setMat4f(std::string("lightSpaceMatrix[") + std::to_string(i).data() + std::string("]"), lightSpaceMatrices[i]);
		m_Shader->setFloat(std::string("frustum[") + std::to_string(i).data() + std::string("]"), frustum[i].second);
		m_Shader->setFloat(std::string("FRUSTUM_SIZE[") + std::to_string(i).data() + std::string("]"), frustumSizes[i]);
	}
	RenderPass::s_Context.shadowOutput.fbo->GetDepthStencilTexture()->Bind(0);
	m_Shader->setInt("shadowMap", 0);

	m_Shader->setInt("shadowType", RenderPass::s_Context.shadowType);
	m_Shader->setFloat("FILTER_STRIDE", RenderPass::s_Context.PCFFilterStride);
	m_Shader->setFloat("LIGHT_WORLD_SIZE", RenderPass::s_Context.PCSSLightSize);

	RenderPass::s_Context.irradianceMapOutput.irradianceCubemap->Bind(1);
	m_Shader->setInt("irrandianceMap", 1);
	RenderPass::s_Context.lPrefilterOutput.lPreFilterCubemap->Bind(2);
	m_Shader->setInt("prefilterMap", 2);
	RenderPass::s_Context.lutOutput.fbo->GetColorTexture()->Bind(3);
	m_Shader->setInt("LUT", 3);
	RenderPass::s_Context.eavgOutput.fbo->GetColorTexture()->Bind(4);
	m_Shader->setInt("Eavg", 4);

	RenderPass::s_Context.geometryOutput.gbuffer->GetColorTextureByIndex(0)->Bind(5);
	m_Shader->setInt("albedoTexture", 5);
	RenderPass::s_Context.geometryOutput.gbuffer->GetColorTextureByIndex(1)->Bind(6);
	m_Shader->setInt("normalTexture", 6);
	RenderPass::s_Context.geometryOutput.gbuffer->GetColorTextureByIndex(2)->Bind(7);
	m_Shader->setInt("positionTexture", 7);
	RenderPass::s_Context.geometryOutput.gbuffer->GetColorTextureByIndex(3)->Bind(8);
	m_Shader->setInt("mixtureTexture", 8);
	RenderPass::s_Context.shadowOutput.fbo->GetColorTexture()->Bind(9);
	m_Shader->setInt("depthSAT", 9);


	// 打开模板测试，只对有片元存在的像素进行着色计算
	RenderCommand::EnableStencilTest();
	RenderCommand::ChangeStencilOption(EMT::RendererAPI::StencilOption::Keep,
		EMT::RendererAPI::StencilOption::Keep, EMT::RendererAPI::StencilOption::Keep);
	RenderCommand::ChangeStencilFunc(EMT::RendererAPI::CompareFunc::Equal, 1, 0xFF);
	RenderCommand::ChangeStencilMask(0x00);
	
	Renderer::RenderNDCPlane();
	
	RenderCommand::DisableStencilTest();
	RenderCommand::EnableDepthTest();
	// 绘制天空盒，深度缓冲是从GBufferGenPass那拷贝来的，之前没渲染的地方会被渲染为天空盒
	Renderer::RenderSkybox(m_Scene);
}

void EMT::DeferredLightingPass::OnWindowResize() {
	RenderPass::s_Context.lightOutput.fbo->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
}
