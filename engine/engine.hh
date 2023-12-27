//
// Created by gl on 12/23/23.
//

#ifndef YAGE_ENGINE_HH
#define YAGE_ENGINE_HH

#include <string>
//#include "squall/squall_vmstd.hpp"
//#define FLECS_CUSTOM_BUILD
//#define FLECS_SYSTEM
//#define FLECS_NO_HTTP
//#include "flecs.h"
//#define YAGE_SWITCH 1


#include "../sokol/sokol_gfx.h"
//#include "../sokol/sokol_log.h"

#define SOKOL_IMGUI_NO_SOKOL_APP

#include "imgui.h"
#include "../sokol_imgui.h"

namespace yage {
    class Engine {
    private:
//        squall::VMStd vm;
//        flecs::world ecs;

        sg_pass_action pass_action = {};
        sg_pass_action imgui_pass_action = {};

        sg_pipeline pipeline = {};
        sg_bindings bindings = {};

        int framecount = 0;


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

        static sg_shader CreateShader(const std::string &shader_name);

    };

} // yaga

#endif //YAGE_ENGINE_HH
