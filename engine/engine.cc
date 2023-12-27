//
// Created by gl on 12/23/23.
//


#include <iostream>
#include "engine.hh"
#include "sokol/sokol_gfx.h"
#include "util.hh"

#include "gen/f_triangle.glsl"
#include "gen/v_triangle.glsl"

namespace yage {
    std::string Engine::GetWindowTitle() {
        return "Yage Game Engine (this is written in code shared by PC and Switch!)";
    }

    Engine::~Engine() = default;

    Engine::Engine() {
        std::cout << "Engine init" << std::endl;
    }

    sg_shader Engine::CreateShader(const std::string &shader_name) {
        sg_shader_desc shader_desc = {};

        shader_desc.vs.source = v_triangleShaderSource[0];
        shader_desc.fs.source = f_triangleShaderSource[0];

        return sg_make_shader(shader_desc);
    }

    void Engine::InitializeGraphics() {
        sg_desc sg_setup_desc = {};
//        sg_setup_desc.logger.func = slog_func;
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

    void Engine::Render(int width, int height) {
        this->framecount++;
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
        ImGui::Text("Frame count: %i", this->framecount);
        ImGui::Text("Platform: %s", getBuild());
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

//#ifndef YAGE_SWITCH
//    void Engine::KeyCallback(int key, int scancode, int action, int mods) {
//        Engine &engine = Engine::GetEngine();
//        engine.TestCallbacks(key);
//    }

    void Engine::CursorPosCallback(double xpos, double ypos) {
        simgui_add_mouse_pos_event((float) xpos, (float) ypos);
    }

    void Engine::MouseButtonCallback(int button, int action, int mods) {
        simgui_add_mouse_button_event(button, action);
    }
//#endif
} // yaga

