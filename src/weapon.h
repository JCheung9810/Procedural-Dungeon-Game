#ifndef WEAPON_H
#define WEAPON_H

#include "raylib.h"

struct Weapon {
    const char* name;
    const char* type;
    
    const char* texturePath;
    int textureFrames;
    Texture2D texture;
    
    float size;
    
    Vector2 positionOffset; 
    float projectileOffset;
    float projectileSize;
    
    float reloadSpeed;
    int magSize;
    int mag;
    float firerate;
    float bulletSpread;
    
    float damage;
    
};

Weapon LoadWeapon(const char* filePath, const char* weaponName);

#endif