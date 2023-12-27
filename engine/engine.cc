//
// Created by gl on 12/23/23.
//


#include <iostream>
#include <fstream>
#include "engine.hh"
#include "sokol/sokol_gfx.h"
//#include "sokol/sokol_log.h"
//#ifndef YAGE_SWITCH

namespace yage {
    std::string Engine::GetWindowTitle() {
        return "Yage Game Engine (this is written in code shared by PC and Switch!)";
    }
#ifndef YAGE_SWITCH

    Engine::~Engine() {
        glfwDestroyWindow(this->mainWindow);
    }
#endif
#ifndef YAGE_SWITCH
    Engine::Engine() {

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow *window = glfwCreateWindow(640, 480, GetWindowTitle().c_str(), 0, 0);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        glfwSetKeyCallback(window, Engine::KeyCallback);
        glfwSetCursorPosCallback(window, Engine::CursorPosCallback);
        glfwSetMouseButtonCallback(window, Engine::MouseButtonCallback);
        this->mainWindow = window;
        std::cout << "Engine init" << std::endl;
    }
#else
Engine::Engine() {
        std::cout << "Engine init" << std::endl;
    }
#endif
//    void Engine::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
////        HandleKeyPress(key, scancode, mods);
//    }

    sg_shader Engine::CreateShader(const std::string &shader_name) {
        sg_shader_desc shader_desc = {};
        auto vsh = "assets/shaders/" + shader_name + ".vsh";
        auto fsh = "assets/shaders/" + shader_name + ".fsh";
        std::ifstream vsh_file(vsh);
        std::ifstream fsh_file(fsh);
        std::string vsh_content((std::istreambuf_iterator<char>(vsh_file)), std::istreambuf_iterator<char>());
        std::string fsh_content((std::istreambuf_iterator<char>(fsh_file)), std::istreambuf_iterator<char>());

        if (!vsh_file.is_open()) {
            std::cerr << "Failed to open vertex shader file: " << vsh << std::endl;
            perror("Error opening file");
        }

        if (!fsh_file.is_open()) {
            std::cerr << "Failed to open fragment shader file: " << fsh << std::endl;
            perror("Error opening file");
        }

        vsh_file.close();
        fsh_file.close();

        shader_desc.vs.source = vsh_content.c_str();
        shader_desc.fs.source = fsh_content.c_str();

        return sg_make_shader(shader_desc);
    }

    void Engine::InitializeGraphics() {
        sg_desc sg_setup_desc = {};
        sg_setup_desc.logger.func = slog_func;
        sg_setup(sg_setup_desc);

        ImGui::CreateContext();
        simgui_desc_t desc = {};
        simgui_setup(desc);

        const float vertices[] = {
                // positions                        // colors
                0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f
        };
        sg_buffer_desc buffer_desc = {};
        buffer_desc.data = SG_RANGE(vertices);
        sg_buffer vertex_buffer = sg_make_buffer(buffer_desc);

        sg_shader_desc shader_desc = {};

        sg_shader shd = Engine::CreateShader("triangle");
        sg_pipeline_desc pipeline_desc = {};
        pipeline_desc.shader = shd;

        sg_vertex_layout_state vls = {};

        vls.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
        vls.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;

        pipeline_desc.layout = vls;
        this->pipeline = sg_make_pipeline(pipeline_desc);

        this->bindings.vertex_buffers[0] = vertex_buffer;

        this->pass_action = {};
        this->pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
        this->pass_action.colors[0].clear_value = {0.0f, 0.5f, 1.0f, 1.0f};

        this->imgui_pass_action = {};
        this->imgui_pass_action.colors[0].load_action = SG_LOADACTION_LOAD;
    }

    void Engine::TestCallbacks(int num) {
        std::cout << "Testing callback works." << num << std::endl;
    }
#ifndef YAGE_SWITCH

    void Engine::DrawLoop() {
        while (!glfwWindowShouldClose(this->mainWindow)) {
            int width, height;
            glfwGetFramebufferSize(this->mainWindow, &width, &height);
            this->Render(width, height);
            glfwSwapBuffers(this->mainWindow);
            glfwPollEvents();
        }
    }
#endif

    void Engine::Render(int width, int height) {
        // ImGui Frame setup
        simgui_frame_desc_t frame_desc = {};
        frame_desc.width = width;
        frame_desc.height = height;
        frame_desc.delta_time = 1.0;
        frame_desc.dpi_scale = 1.0;
        simgui_new_frame(frame_desc);

        ImGui::SetNextWindowPos((ImVec2) {10, 10}, ImGuiCond_Once, (ImVec2) {0, 0});
        ImGui::SetNextWindowSize((ImVec2) {400, 100}, ImGuiCond_Once);

        // Begin frame render
        ImGui::Begin("Yet Another Game Engine", nullptr, ImGuiWindowFlags_None);
        this->RenderScene(width, height);

        // End frame render
        ImGui::End();
        sg_begin_default_pass(&this->imgui_pass_action, width, height);
        simgui_render();
        sg_end_pass();

        sg_commit();
    }

    void Engine::RenderScene(int width, int height) {
        sg_begin_default_pass(&this->pass_action, width, height);
        sg_apply_pipeline(this->pipeline);
        sg_apply_bindings(&this->bindings);
        sg_draw(0, 3, 1);
        sg_end_pass();
    }

#ifndef YAGE_SWITCH
    void Engine::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        Engine &engine = Engine::GetEngine();
        engine.TestCallbacks(key);
    }

    void Engine::CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
//        Engine::GetEngine().TestCallbacks(key);
        simgui_add_mouse_pos_event((float) xpos, (float) ypos);
    }
    void Engine::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
        simgui_add_mouse_button_event(button, action);
    }
#endif
} // yaga

