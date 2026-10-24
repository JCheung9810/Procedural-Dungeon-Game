#ifndef WEAPON_H
#define WEAPON_H

#include "raylib.h"

struct Weapon {
    char* name;
    char* type;
    
    char* texturePath;
    Texture2D texture;
    
    float size;
    float positionOffset; 
    
    float projectileOffset;
    
};

Weapon LoadWeapon(const char* filePath, const char* weaponName);

#endif