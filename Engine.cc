//
// Created by gl on 12/23/23.
//


#include <iostream>
#include "Engine.hh"
//#include "sokol/sokol_gfx.h"
#include "sokol/sokol_log.h"

namespace yaga {
std::string Engine::GetWindowTitle() {
  return "Yaga Game";
}

Engine::Engine() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  this->mainWindow = glfwCreateWindow(640, 480, GetWindowTitle().c_str(), 0, 0);
  glfwMakeContextCurrent(this->mainWindow);
  glfwSwapInterval(1);

  std::cout << "Engine init" << std::endl;
}

void Engine::InitalizeGraphics() {
  sg_desc sg_setup_desc;
  sg_setup_desc.logger.func = slog_func;
  sg_setup(sg_setup_desc);
  const float vertices[] = {
      // positions            // colors
      0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f
  };
  sg_buffer_desc buffer_desc;
  buffer_desc.data = SG_RANGE(vertices);
  sg_buffer vbuf = sg_make_buffer(buffer_desc);

  sg_shader_desc shader_desc;
  shader_desc.vs.source = "#version 330\n"
                          "layout(location=0) in vec4 position;\n"
                          "layout(location=1) in vec4 color0;\n"
                          "out vec4 color;\n"
                          "void main() {\n"
                          "  gl_Position = position;\n"
                          "  color = color0;\n"
                          "}\n",
      shader_desc.fs.source =
          "#version 330\n"
          "in vec4 color;\n"
          "out vec4 frag_color;\n"
          "void main() {\n"
          "  frag_color = color;\n"
          "}\n";
  sg_shader shd = sg_make_shader(shader_desc);

  sg_pipeline_desc pipeline_desc;
  pipeline_desc.shader = shd;

//  sg_vertex_attr_state vas;

  sg_vertex_layout_state vls;

  vls.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
  vls.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;

  pipeline_desc.layout = vls;
  this->pipeline = sg_make_pipeline(pipeline_desc);

  this->bindings.vertex_buffers[0] = vbuf;
  this->pass_action = {0};
}
  void Engine::DrawLoop(){
    while (!glfwWindowShouldClose(this->mainWindow)) {
      int cur_width, cur_height;
      glfwGetFramebufferSize(this->mainWindow, &cur_width, &cur_height);
      sg_begin_default_pass(&this->pass_action, cur_width, cur_height);
      sg_apply_pipeline(this->pipeline);
      sg_apply_bindings(&this->bindings);
      sg_draw(0, 3, 1);
      sg_end_pass();
      sg_commit();
      glfwSwapBuffers(this->mainWindow);
      glfwPollEvents();
    }
}
} // yaga