#include "emtpch.h"
#include "WindowsInput.h"
#include "EMT/Application.h"
#include <GLFW/glfw3.h>

namespace EMT {
    Input* Input::s_Instance = new WindowsInput();

    bool WindowsInput::isKeyPressedImpl(int key)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow()->GetNativeWindow());
        int statu = glfwGetKey(window, key);

        return statu == GLFW_PRESS || statu == GLFW_REPEAT;
    }

    bool WindowsInput::isMouseBottomPressedImpl(int button)
    {
        // 0->Êó±ê×ó¼ü      1->Êó±êÓÒ¼ü     2->Êó±êÖÐ¼ü
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow()->GetNativeWindow());
        int statu = glfwGetMouseButton(window, button);

        return statu == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::GetMousePosImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow()->GetNativeWindow());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return { (float)x,(float)y };
    }
}

