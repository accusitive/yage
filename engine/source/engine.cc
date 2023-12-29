//
// Created by gl on 12/23/23.
//


#include <iostream>
#include <random>
#include <chrono>

#include "engine.hh"
#include "util.hh"
#include "../gen/triangle.glsl.h"
#include "box2d/box2d.h"
#include "components.hh"
#include "../gen/camel.png.h"

int decodePNG(std::vector<unsigned char> &out_image, unsigned long &image_width, unsigned long &image_height,
              const unsigned char *in_png, size_t in_size, bool convert_to_rgba32 = true);


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


    void Engine::RenderQuad(float x, float y, float width, float height) {
        float vertices[] = {
                x, y,                   0.0f, 0.0f, // top left
                x + width, y,           1.0f, 0.0f, // top right
                x, y + height,          0.0f, 1.0f, // bottom left

                x, y + height,          0.0f, 1.0f, // bottom left
                x + width, y + height,  1.0f, 1.0f, // bottom right
                x + width, y,           1.0f, 0.0f, // top right
        };

        const int num_verts = 6;
        const int vert_size = 4;
        this->scene.insert(this->scene.end(), vertices, vertices + (num_verts * vert_size));
    }

    void Engine::PopulateWithDebugEntities() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-YAGE_WORLD_SIZE, YAGE_WORLD_SIZE);
        for (auto i = 0u; i < YAGE_NUMBER_DEBUG_ENTITIES; ++i) {
            auto x = (float) dis(gen);
            auto y = (float) dis(gen);
            y = y + YAGE_WORLD_SIZE;
            y /= 2.0f;

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(x, y);
            b2Body *body = world.CreateBody(&bodyDef);

            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(YAGE_UNIT_SIZE, YAGE_UNIT_SIZE);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;

            body->CreateFixture(&fixtureDef);

            const auto entity = this->registry.create();
            registry.emplace<ComponentPosition>(entity, x, y);
            registry.emplace<ComponentSprite>(entity);
            registry.emplace<ComponentPhysicsBody>(entity, body);


        }
    };

    void Engine::CreatePlayer() {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(0.0f, 0.0f);
        b2Body *body = world.CreateBody(&bodyDef);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(YAGE_UNIT_SIZE, YAGE_UNIT_SIZE);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 1.f;

        body->CreateFixture(&fixtureDef);

        const auto entity = this->registry.create();
        registry.emplace<ComponentPosition>(entity, 0.0f, 0.0f);
        registry.emplace<ComponentSprite>(entity);
        registry.emplace<ComponentPhysicsBody>(entity, body);
        registry.emplace<ComponentPlayer>(entity);
        registry.emplace<ComponentPlayerInput>(entity, ComponentPlayerInput{
                .move_speed = 10.0f,
                .jump_height = 25.0f,
        });


    }

    void Engine::CreateGroundBox() {
        b2BodyDef ground_body_def;
        ground_body_def.position.Set(-YAGE_WORLD_SIZE, 0.0f);
        b2Body *ground_body = this->world.CreateBody(&ground_body_def);
        this->ground_body = ground_body;
        b2PolygonShape ground_box;
        ground_box.SetAsBox(YAGE_WORLD_SIZE * 2.0f, 10.0f);
        ground_body->CreateFixture(&ground_box, 0.0f);


    }

    void Engine::Initialize() {
        this->PopulateWithDebugEntities();
        this->CreatePlayer();
        this->CreateGroundBox();
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
        pipeline_desc.layout.attrs[ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT2;
        pipeline_desc.layout.attrs[ATTR_vs_texcoord].format = SG_VERTEXFORMAT_FLOAT2;

        sg_buffer_desc buffer_desc = {};
        buffer_desc.usage = SG_USAGE_STREAM;
        buffer_desc.size = (32768) * sizeof(float); // BUFFERSIZE
        sg_buffer vbx = sg_make_buffer(buffer_desc);

        std::vector<unsigned char> camel_pixels;
        auto t = &camel_pixels;
        unsigned long w;
        unsigned long h;
        decodePNG(*t, w, h, assets_textures_camel_png, assets_textures_camel_png_len, true);

        sg_image_data camel_image_data = {};
        camel_image_data.subimage[0][0] = sg_range(camel_pixels.data(), camel_pixels.size());

        sg_image_desc camel_image_desc = {};
        camel_image_desc.data = camel_image_data;
        camel_image_desc.width = 64;
        camel_image_desc.height = 64;
        camel_image_desc.label = "camel-texture";

        sg_sampler_desc camel_sampler_desc = {};
        camel_sampler_desc.label = "camel-sampler";

        this->bindings.vertex_buffers[0] = vbx;
        this->bindings.fs.images[SLOT_tex] = sg_make_image(camel_image_desc);
        this->bindings.fs.samplers[SLOT_smp] = sg_make_sampler(camel_sampler_desc);

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
        auto start = std::chrono::high_resolution_clock::now();


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
        ImGui::Begin("Camel Water Simulator on Y.A.G.E", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Platform: %s", getBuild());


        this->HandleMovement();
        this->world.Step(1.0f / 60.0f, 6, 2);
        this->RenderScene(width, height);

        // End frame render
        ImGui::End();

        sg_begin_default_pass(&this->imgui_pass_action, width, height);
        simgui_render();
        sg_end_pass();

        sg_commit();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        double frame_time = elapsed.count();
        this->previous_frametime = frame_time;
    }

    void Engine::RenderScene(int width, int height) {
//        RenderDebugQuad(64.0f, 64.0f, 24.0f, 24.0f);

        this->registry.view<const ComponentSprite, const ComponentPosition>().each(
                [this](const auto &sprite, ComponentPosition pos) {
                    this->RenderQuad(pos.x, pos.y, YAGE_UNIT_SIZE * 2.0f, YAGE_UNIT_SIZE * 2.0f);
                });
        this->RenderQuad(this->ground_body->GetPosition().x, this->ground_body->GetPosition().y, YAGE_WORLD_SIZE * 2.0f,
                         11.56f);
        registry.view<ComponentPosition, ComponentPhysicsBody>().each(
                [this](auto entity, ComponentPosition &pos, ComponentPhysicsBody &physics_body) {
                    this->registry.patch<ComponentPosition>(entity, [physics_body](ComponentPosition &pos) {
                        pos.x = physics_body.body->GetPosition().x;
                        pos.y = physics_body.body->GetPosition().y;
                    });
                });
        // Update scene data

        auto scene_size = this->scene.size() * sizeof(float);

        if(ImGui::TreeNodeEx("Rendering Info"), ImGuiTreeNodeFlags_DefaultOpen) {
            ImGui::Text("Frame count: %i", this->frame_count);
            ImGui::Text("Vertices: %zu", this->scene.size() / 4);
            ImGui::Text("Scene size: %lu bytes", scene_size);
            ImGui::Text("Previous: %f", this->previous_frametime);
        }

        sg_update_buffer(this->vertex_buffer, {this->scene.data(), scene_size});
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

    bool Engine::IsPhysicsBodyOnGround(b2Body *physics_body) {
        auto contact = physics_body->GetContactList();
        while (contact != nullptr) {
            if (contact->other == this->ground_body) {
                return true;
            }
            contact = contact->next;
        }
        return false;
    }

    void Engine::HandleMovement() {
        this->registry.view<const ComponentPlayerInput, const ComponentPhysicsBody>().each(
                [this](auto entity, ComponentPlayerInput player_input, ComponentPhysicsBody physics_body) {
                    ImGui::TreeNodeEx("Player Movement", ImGuiTreeNodeFlags_DefaultOpen);

                    ImGui::Text("Should Jump: %b", player_input.should_jump);
                    ImGui::Text("Horizontal: %f", player_input.horizontal);
                    ImGui::Text("Debug stick: %i", player_input.debug_stick);

                    ImGui::TreePop();

                    if (player_input.should_jump) {
                        this->registry.patch<ComponentPlayerInput>(entity, [](ComponentPlayerInput &player_input) {
                            player_input.should_jump = false;
                        });
                        physics_body.body->SetLinearVelocity(b2Vec2(0.0f, player_input.jump_height));

                    }
                    if (player_input.horizontal != 0.0f) {
                        physics_body.body->SetLinearVelocity(
                                b2Vec2(player_input.horizontal * player_input.move_speed,
                                       physics_body.body->GetLinearVelocity().y));
                    }
                });
    }

    void Engine::InputHandleJump() {
        this->registry.view<const ComponentPlayerInput, const ComponentPhysicsBody>().each(
                [this](auto entity, ComponentPlayerInput player_input, ComponentPhysicsBody physics_body) {
                    auto grounded = this->IsPhysicsBodyOnGround(physics_body.body);
                    this->registry.patch<ComponentPlayerInput>(entity, [grounded](auto &player_input) {
                        player_input.should_jump = grounded;
                    });
                });
    }

    void Engine::DebugStick(int32_t stick) {
        this->registry.view<const ComponentPlayerInput, const ComponentPhysicsBody>().each(
                [this, stick](auto entity, ComponentPlayerInput player_input, ComponentPhysicsBody physics_body) {
                    this->registry.patch<ComponentPlayerInput>(entity, [stick](auto &player_input) {
                        player_input.debug_stick = stick;
                    });
                });
    }

    void Engine::InputHandleHorizontal(float x) {
        this->registry.view<const ComponentPlayerInput, const ComponentPhysicsBody>().each(
                [this, x](auto entity, ComponentPlayerInput player_input, ComponentPhysicsBody physics_body) {
                    this->registry.patch<ComponentPlayerInput>(entity, [x](auto &player_input) {
                        player_input.horizontal = x;
                    });
                });
    }
//#endif
} // yaga

