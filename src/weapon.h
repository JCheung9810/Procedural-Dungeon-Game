#pragma once

#include "raylib.h"

//Forward declaration
struct Player;

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
    
    float reloadTime;
    float reloadTimer;
    bool reloading;
    
    int magSize;
    int mag;
    float firerate;
    float bulletSpread;
    
    float damage;
    
};

Weapon LoadWeapon(const char* filePath, const char* weaponName);
void ReloadWeapon(Player& player);
void UpdateWeapon(Player& player);
