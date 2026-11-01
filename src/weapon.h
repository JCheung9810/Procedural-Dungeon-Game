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
    
    float reloadTime;       //"Reload Rate"
    float reloadTimer;
    bool reloading;
    
    int magSize;
    int mag;
    
    float fireTime;         //Rate of fire in seconds = 1 proj / n seconds
    float fireTimer;        //Timer for firing
    
    float firerate;         //"Firerate" = 1/ fireTime =   n proj / 1 second
    
    float bulletSpread;
    
    float damage;
    
};

Weapon LoadWeapon(const char* filePath, const char* weaponName);
void ReloadWeapon(Player& player);
void UpdateWeapon(Player& player);
