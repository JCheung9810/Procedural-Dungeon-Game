#pragma once

#include "assetManager.h"

#include "raylib.h"

//Forward declaration
struct Player;

enum class FireMode {
    SINGLE,
    AUTOMATIC
};

struct Weapon {
    const char* name;
    const char* type;
    
    const char* texturePath;
    int textureFrames;
    Texture2D texture;
    
    const char* hudTexturePath;
    Texture2D hudTexture;
    
    const char* shotSoundPath;
    Sound shotSound;
    
    const char* reloadSoundPath;
    Sound reloadSound;
    
    float size;
    
    Vector2 positionOffset; 
    float projectileOffset;
    float projectileSize;
    
    int numProjectiles;     //Projectiles in 1 shot
    
    float reloadTime;       //"Reload Rate"
    float reloadTimer;
    bool reloading;
    
    int magSize;            //Max number of bullets in mag
    int mag;                //Number of bullets in mag
    
    float fireTime;         //Rate of fire in seconds = 1 proj / n seconds
    float fireTimer;        //Timer for firing
    
    float firerate;         //"Firerate" = 1/ fireTime =   n proj / 1 second
    
    FireMode fireMode;
    
    float bulletSpread;     //Range of angle deviating from player to target        5 = +- 5 degrees
    
    float damage;
    
};

Weapon LoadWeapon(const char* filePath, const char* weaponName, AssetManager& assets);
void ReloadWeapon(Player& player);
void UpdateWeapon(Player& player);
