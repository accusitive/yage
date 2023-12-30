//
// Created by gl on 12/30/23.
//

#ifndef YAGE_RENDERPASS_HH
#define YAGE_RENDERPASS_HH

#include "HandmadeMath.h"

//#include "../sokol/sokol_gfx.h"
#include "../gen/camel.glsl.h"
#include "../gen/sand.glsl.h"

#include <vector>

namespace yage {

    class RenderPass {
    public:
        sg_pipeline pipeline = {};
        sg_bindings bindings = {};
        sg_buffer vertex_buffer = {};
        sg_pass render_pass = {};
        std::vector<float> scene;


        virtual sg_pipeline_desc get_pipeline_desc() {
            return {};
        }

        virtual int get_uniform_slot() {
            return 0;
        }
        RenderPass() = default;
        RenderPass(sg_image_desc tex, const sg_shader_desc *shad, sg_pipeline_desc pipeline_desc) {

            // Vertex Buffer
            sg_buffer_desc buffer_desc = {};
            buffer_desc.usage = SG_USAGE_STREAM;
            buffer_desc.size = (32768) * sizeof(float);
            this->vertex_buffer = sg_make_buffer(buffer_desc);

            // Sampler
            sg_sampler_desc camel_sampler_desc = {};
            camel_sampler_desc.label = "some-sampler";

            // Pipeline
            sg_shader shd = sg_make_shader(shad);
            pipeline_desc.shader = shd;

            this->bindings.vertex_buffers[0] = this->vertex_buffer;
            this->bindings.fs.images[SLOT_tex] = sg_make_image(tex);
            this->bindings.fs.samplers[SLOT_smp] = sg_make_sampler(camel_sampler_desc);
            this->pipeline = sg_make_pipeline(pipeline_desc);

        }
        [[nodiscard]] unsigned long get_scene_size() const {
            return this->scene.size() * sizeof(float);
        }

        void begin_frame() {
            this->scene.clear();
        }
        void update_buffers() {

            sg_update_buffer(this->vertex_buffer, {this->scene.data(), this->get_scene_size()});
        }
        void draw(sg_range uniform_data) {
            sg_apply_pipeline(this->pipeline);
            sg_apply_bindings(this->bindings);
            sg_apply_uniforms(SG_SHADERSTAGE_VS, get_uniform_slot(), uniform_data);
            sg_draw(0, (int) this->get_scene_size(), 1);
        }

    };

    class CamelRenderPass : public RenderPass {

    public:
        CamelRenderPass() = default;

        sg_pipeline_desc get_pipeline_desc() override {
            sg_pipeline_desc pipeline_desc = {};
            pipeline_desc.layout.attrs[ATTR_camel_vs_position].format = SG_VERTEXFORMAT_FLOAT2;
            pipeline_desc.layout.attrs[ATTR_camel_vs_texcoord].format = SG_VERTEXFORMAT_FLOAT2;
            return pipeline_desc;
        }
        int get_uniform_slot() override {
            return SLOT_camel_uniforms;
        }
        explicit CamelRenderPass(sg_image_desc tex) :
                RenderPass(tex, camel_shader_desc(sg_query_backend()), get_pipeline_desc()) {
        }


    };

    class SandRenderPass : public RenderPass {

    public:
        SandRenderPass() = default;

        sg_pipeline_desc get_pipeline_desc() override {
            sg_pipeline_desc pipeline_desc = {};
            pipeline_desc.layout.attrs[ATTR_sand_vs_position].format = SG_VERTEXFORMAT_FLOAT2;
            pipeline_desc.layout.attrs[ATTR_sand_vs_texcoord].format = SG_VERTEXFORMAT_FLOAT2;
            return pipeline_desc;
        }
        int get_uniform_slot() override  {
            return SLOT_sand_uniforms;
        }

        explicit SandRenderPass(sg_image_desc tex) :
                RenderPass(tex, sand_shader_desc(sg_query_backend()), get_pipeline_desc()) {
        }
    };
} // yage

#endif //YAGE_RENDERPASS_HH
