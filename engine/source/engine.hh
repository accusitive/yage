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
#include "HandmadeMath.h"
#include "box2d/box2d.h"

#define YAGE_WORLD_SIZE 64.0f
#define YAGE_UNIT_SIZE (1/YAGE_WORLD_SIZE * 100.0f)
#define YAGE_NUMBER_DEBUG_ENTITIES 8u

namespace yage {
    class Engine {
    private:
        b2World world = b2World(b2Vec2(0.0f, -(YAGE_UNIT_SIZE * YAGE_WORLD_SIZE/2.0)));
        b2Body* ground_body;
        entt::registry registry;
//        ShaderResourceManager shader_resource_manager;
        HMM_Mat4 camera_projection = HMM_Orthographic_LH_NO(-YAGE_WORLD_SIZE, YAGE_WORLD_SIZE, -YAGE_WORLD_SIZE, YAGE_WORLD_SIZE, 0.1f, 128.0f);
        sg_pass_action pass_action = {};
        sg_pass_action imgui_pass_action = {};

        sg_pipeline pipeline = {};
        sg_bindings bindings = {};

        std::vector<float> scene;
        sg_buffer vertex_buffer = {};
        int frame_count = 0;
        double previous_frametime = 0.0F;

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

        void Initialize();


        void Render(int width, int height);

        static void KeyCallback(int key, int scancode, int action, int mods);

        static void CursorPosCallback(double xpos, double ypos);

        static void MouseButtonCallback(int button, int action, int mods);

        void TestCallbacks(int num);

        void RenderScene(int width, int height);

        static sg_shader create_shader_program();


        void RenderQuad(float x, float y, float width, float height);

        void RenderDebugQuad(float x, float y, float width, float height);

        void PopulateWithDebugEntities();

        void CreateGroundBox();

        void InputHandleJump();

        void CreatePlayer();

        bool IsPhysicsBodyOnGround(b2Body* physics_body);

        void InputHandleHorizontal(float x);

        void HandleMovement();

        void DebugStick(int32_t stick);
    };

} // yaga

#endif //YAGE_ENGINE_HH
