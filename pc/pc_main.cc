#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define SOKOL_IMGUI_NO_SOKOL_APP

#include "engine.hh"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

int main(int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(640, 480, yage::Engine::GetWindowTitle().c_str(), 0, 0);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
//    glfwSetKeyCallback(window, Engine::KeyCallback);
//    glfwSetCursorPosCallback(window, Engine::CursorPosCallback);
//    glfwSetMouseButtonCallback(window, Engine::MouseButtonCallback);
//    this->mainWindow = window;
//    std::cout << "Engine init" << std::endl;

    auto e = &yage::Engine::GetEngine();
    e->InitializeGraphics();

//    e->DrawLoop();
        while (!glfwWindowShouldClose(window)) {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            e->Render(width, height);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    glfwDestroyWindow(window);

}