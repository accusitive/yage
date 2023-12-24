#include <iostream>

#define SOKOL_IMPL
#define SOKOL_GLCORE33
//#include "sokol/sokol_gfx.h"

#include "Engine.hh"

struct Position {
  float x, y;
};

struct Velocity {
  float x, y;
};
struct Identifier {
  int32_t name;
};
int main(int argc, char *argv[]) {


//    ecs.component<Identifier>().member<int32_t>("identifier");
//
//    ecs.system<Position, const Velocity, const Identifier>()
//            .each([](Position &p, const Velocity &v, const Identifier &id) {
//                std::cout << "Entity" << id.name << std::endl;
//                p.x += v.x;
//                p.y += v.y;
//            });
//    auto e = ecs.entity()
//            .set([](Position &p, Velocity &v, Identifier &i) {
//                p = {10, 20};
//                v = {1, 2};
//                i = {0};
//            });

//    while (ecs.progress()) { }

//    vm.defun("create_entity", [&ecs]() {
//        auto id = ecs_new_id(ecs.m_world);
////        ecs_set()
//        return id;
//    });

//    vm.dostring(R"(
//        print("Hello from squirrel");
//        function run() {
//            print("Created entity: " + create_entity() + "\n");
//        }
//        run();
//        run();
//        run();
//        run();
//        run();
//        run();
//            )");
//    ecs.progress();

  auto *e = new yaga::Engine();

  e->InitalizeGraphics();
  e->DrawLoop();

}