//
// Created by gl on 12/29/23.
//

#ifndef YAGE_COMPONENTS_HH
#define YAGE_COMPONENTS_HH

#include "box2d/box2d.h"

struct ComponentPosition {
    float x;
    float y;
};
struct ComponentSprite {

};
struct ComponentPhysicsBody {
    b2Body *body;
};
struct ComponentPlayer {
};
// Marker component to find which entity(s) should take player input
struct ComponentPlayerInput {
    float move_speed = 0.0f;
    float jump_height = 0.0f;
    bool should_jump = false;
    float horizontal = 0.0f;
    int32_t debug_stick = 0;
};
// Movement speed data

#endif //YAGE_COMPONENTS_HH
