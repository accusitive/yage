#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define SOKOL_IMGUI_NO_SOKOL_APP

#include "source/engine.hh"
#include <iostream>

#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"
void CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    yage::Engine::CursorPosCallback(xpos, ypos);
}

void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    yage::Engine::MouseButtonCallback(button, action, mods);
}
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        yage::Engine::GetEngine().TempHandleJump();
    }

}
float GetMovementAxis(GLFWwindow* window) {
    auto a = glfwGetKey(window, GLFW_KEY_A);
    auto d = glfwGetKey(window, GLFW_KEY_D);

    if(a && !d) {
        return -1.0f;
    } else if(!a && d) {
        return 1.0f;
    } else {
        return 0.0f;
    }
}


int main(int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(640, 480, yage::Engine::GetWindowTitle().c_str(), 0, 0);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    std::cout << "Engine init" << std::endl;

    auto e = &yage::Engine::GetEngine();
    e->Initialize();
    // Draw loop
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        e->HandleMovement(GetMovementAxis(window));
        e->Render(width, height);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
}

