#pragma once

#include "EMT/Application.h"

// 日志系统
#include "EMT/Log.h"

// 层栈
#include "EMT/Layer.h"
#include "EMT/ImGui/ImGuiLayer.h"

// 输入
#include "EMT/Input.h"
#include "EMT/KeyCode.h"
#include "EMT/MouseButton.h"

// 渲染器
#include "EMT/Renderer/Buffer.h"
#include "EMT/Renderer/Camera.h"
#include "EMT/Renderer/GraphicsContext.h"
#include "EMT/Renderer/RenderCommand.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/Renderer/Shader.h"
#include "EMT/Renderer/VertexArray.h"
#include "EMT/Renderer/Texture.h"

// -------------入口点--------------
// 这个头文件定义的是（游戏的）main函数
#include "EMT/EntryPoint.h"
// ------------------ --------------