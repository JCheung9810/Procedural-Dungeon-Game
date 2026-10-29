#ifndef DEBUG_H
#define DEBUG_H

#include "raylib.h"

struct Debug {
    bool active = true;
    
    bool speed = false;
    bool wall = false;
    bool cam = false;
    bool hitbox = false;

};

#endif