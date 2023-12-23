#include <iostream>
#include <squall/squall_vmstd.hpp>

struct Position {
    float x, y;
};

struct Velocity {
    float x, y;
};

int main(int argc, char *argv[]) {
    squall::VMStd vm;
    flecs::world ecs;

    ecs.system<Position, const Velocity>()
            .each([](Position& p, const Velocity& v) {
                p.x += v.x;
                p.y += v.y;
            });

    auto e = ecs.entity()
            .set([](Position& p, Velocity& v) {
                p = {10, 20};
                v = {1, 2};
            });

    while (ecs.progress()) { }
}