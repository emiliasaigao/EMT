#include "emtpch.h"
#include "OpenGLAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace EMT {
	void OpenGLAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color. a);
	}

	void OpenGLAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLAPI::DrawIndexed(const EMT::Ref<VertexArray>& vertexArray)
	{
		EMT_ASSERT(vertexArray->GetElementBuffer(), "vertexArray do not have a ebo");
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetElementBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLAPI::DrawArray(const Ref<VertexArray>& vertexArray)
	{
		EMT_ASSERT(vertexArray->GetVertexBuffers().size(), "vertexArray do not bind a vbo");
		vertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexBuffers()[0]->GetCount());
	}
	
	float OpenGLAPI::GetTime()
	{
		return glfwGetTime();
	}
	
	void OpenGLAPI::EnableDepthTest() {
		glEnable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void OpenGLAPI::DisableDepthTest() {
		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLAPI::EnableMultiSample() {
		glEnable(GL_MULTISAMPLE);
	}

	void OpenGLAPI::DisableMultiSample() {
		glDisable(GL_MULTISAMPLE);
	}

	void OpenGLAPI::EnableCubeMapSeampless() {
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	void OpenGLAPI::SetViewport(int x, int y, int width, int height) {
		glViewport(x, y, width, height);
	}

	void OpenGLAPI::ChangeDepthFunc(DepthFunc func) {
		glDepthFunc(GL_NEVER + (unsigned int)func);
	}
}