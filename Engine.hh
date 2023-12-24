//
// Created by gl on 12/23/23.
//

#ifndef YAGE_ENGINE_HH
#define YAGE_ENGINE_HH

#include <squall/squall_vmstd.hpp>
#include <flecs.h>


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace yaga {

class Engine {
 private:
  squall::VMStd vm;
  flecs::world ecs;
  GLFWwindow *mainWindow{};

  sg_pass_action pass_action;
  sg_pipeline pipeline;
  sg_bindings bindings;

 public:
  Engine();
  ~Engine() = default;

  static std::string GetWindowTitle();
  void InitalizeGraphics();
  void DrawLoop();
};

} // yaga

#endif //YAGE_ENGINE_HH
