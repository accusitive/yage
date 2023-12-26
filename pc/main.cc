#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define SOKOL_IMGUI_NO_SOKOL_APP

#include "engine.hh"

int main(int argc, char *argv[]) {
    auto e = &yage::Engine::GetEngine();
    e->InitializeGraphics();
    e->DrawLoop();
}