//
// Created by gl on 12/23/23.
//

#ifndef YAGE_ENGINE_HH
#define YAGE_ENGINE_HH

#include <string>
#include <entt/entt.hpp>

#include "../sokol/sokol_gfx.h"
//#include "../sokol/sokol_log.h"
#define SOKOL_IMGUI_NO_SOKOL_APP

#include "imgui.h"
#include "../../sokol_imgui.h"
#include "resource.hh"
#include "HandmadeMath.h"

namespace yage {
    class Engine {
    private:
        entt::registry registry;
        ShaderResourceManager shader_resource_manager;
        HMM_Mat4 camera_projection;
        sg_pass_action pass_action = {};
        sg_pass_action imgui_pass_action = {};

        sg_pipeline pipeline = {};
        sg_bindings bindings = {};

        int framecount = 0;

        static void
        SokolLog(const char *tag, uint32_t log_level, uint32_t log_item, const char *message, uint32_t line_nr,
                 const char *filename, void *user_data);

    public:
        static Engine &GetEngine() {
            static Engine instance;
            return instance;
        }

        Engine();

        ~Engine();

        Engine(const Engine &) = delete;

        Engine &operator=(const Engine &) = delete;

        static std::string GetWindowTitle();

        void InitializeGraphics();


        void Render(int width, int height);

        static void KeyCallback(int key, int scancode, int action, int mods);

        static void CursorPosCallback(double xpos, double ypos);

        static void MouseButtonCallback(int button, int action, int mods);

        void TestCallbacks(int num);

        void RenderScene(int width, int height);

        static sg_shader create_shader_program(const ShaderResource &vs, const ShaderResource &fs);


    };

} // yaga

#endif //YAGE_ENGINE_HH
