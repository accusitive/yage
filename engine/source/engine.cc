//
// Created by gl on 12/23/23.
//


#include <iostream>
#include "engine.hh"

#ifndef YAGE_SWITCH

#include "../sokol/sokol_log.h"

#endif

#include "util.hh"
//#include "resource.hh"
#include "../gen/triangle.glsl.h"

namespace yage {
    std::string Engine::GetWindowTitle() {
        return "Yage Game Engine (this is written in code shared by PC and Switch!)";
    }

    Engine::~Engine() = default;

    Engine::Engine() {
        std::cout << "Engine init" << std::endl;
//        this->shader_resource_manager.register_builtin_shader_resources();
    }

    void Engine::SokolLog(const char *tag, uint32_t log_level, uint32_t log_item, const char *message, uint32_t line_nr,
                          const char *filename, void *user_data) {
        std::cout << "[" << tag << "]" << " `" << message << "` @" << filename << ":" << line_nr << ". @" << user_data
                  << '@' << std::endl;
    }

    void Engine::RenderDebugQuad(float x, float y, float width, float height) {
        float z = 0.0f;
        float h = 64.0f;
        float vertices[] = {
                z, z, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top left
                h, z, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top right
                z, h, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left

                z, h, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
                h, h, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
                h, z, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
        };
        const int num_verts = 6;
        const int vert_size = 7;
        this->scene.insert(this->scene.end(), vertices, vertices + (num_verts * vert_size));
    }

    void Engine::RenderQuad(float x, float y, float width, float height) {
        float vertices[] = {
                x, y, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top left
                x + width, y, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top right
                x, y + height, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left

                x, y + height, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
                x + width, y + height, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
                x + width, y, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
        };

        const int num_verts = 6;
        const int vert_size = 7;
        this->scene.insert(this->scene.end(), vertices, vertices + (num_verts * vert_size));
    }

    void Engine::Initialize() {
        sg_desc sg_setup_desc = {};
        sg_setup_desc.uniform_buffer_size = sizeof(HMM_Mat4);
        sg_setup_desc.logger.func = Engine::SokolLog;
        sg_setup(sg_setup_desc);

        ImGui::CreateContext();
        simgui_desc_t desc = {};
        simgui_setup(desc);

        sg_shader shd = sg_make_shader(triangle_shader_desc(sg_query_backend()));
        sg_pipeline_desc pipeline_desc = {};
        pipeline_desc.shader = shd;
        pipeline_desc.layout.attrs[ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3;
        pipeline_desc.layout.attrs[ATTR_vs_color0].format = SG_VERTEXFORMAT_FLOAT4;

        sg_buffer_desc buffer_desc = {};
        buffer_desc.usage = SG_USAGE_STREAM;
        buffer_desc.size = (6 * 100) * sizeof(float); // BUFFERSIZE. In theory enough to support 100 quads
        sg_buffer vbx = sg_make_buffer(buffer_desc);
        this->bindings.vertex_buffers[0] = vbx;

        this->pipeline = sg_make_pipeline(pipeline_desc);

        this->pass_action = {};
        this->pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
        this->pass_action.colors[0].clear_value = {0.0f, 0.5f, 1.0f, 1.0f};
        this->imgui_pass_action = {};
        this->imgui_pass_action.colors[0].load_action = SG_LOADACTION_LOAD;
        this->vertex_buffer = vbx;
    }

    void Engine::TestCallbacks(int num) {
        std::cout << "Testing callback works." << num << std::endl;
    }

    void Engine::Render(int width, int height) {
        this->frame_count++;
        this->scene.clear();
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
        ImGui::Text("Frame count: %i", this->frame_count);
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
        RenderDebugQuad(64.0f, 64.0f, 24.0f, 24.0f);

        RenderQuad(-32.0f, -32.0f, 4.0f, 4.0f);
        RenderQuad(-36.0f, -32.0f, 4.0f, 4.0f);

        // Update scene data
        ImGui::Text("Vertices: %zu", this->scene.size() / 7);
        auto scene_size = this->scene.size() * sizeof(float);
        ImGui::Text("scene_size: %lu bytes", scene_size);

        sg_update_buffer(this->vertex_buffer, {this->scene.data(), scene_size});
        this->bindings.vertex_buffers[0] = this->vertex_buffer;
        sg_begin_default_pass(&this->pass_action, width, height);
        sg_apply_pipeline(this->pipeline);
        sg_apply_bindings(this->bindings);
        sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_per_frame, SG_RANGE_REF(this->camera_projection));
        sg_draw(0, (int) this->scene.size(), 1);
        sg_end_pass();
    }

    void Engine::CursorPosCallback(double xpos, double ypos) {
        simgui_add_mouse_pos_event((float) xpos, (float) ypos);
    }

    void Engine::MouseButtonCallback(int button, int action, int mods) {
        simgui_add_mouse_button_event(button, action);
    }
//#endif
} // yaga

