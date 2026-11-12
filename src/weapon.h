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
    const char* name;               //Display name (specific)
    const char* type;               //General weapon type (Pistol, Smg, etc..)
    
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
    
    Vector2 positionOffset;         //Offset for gun to appear centered around entity
    float projectileOffset;         //Offset for projectile to come out of the barrel (distance from center rotation/hypotenuse)
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
    
    FireMode fireMode;      //SINGLE, AUTOMATIC
    
    float bulletSpread;     //Range of angle deviating from player to target        5 = +- 5 degrees
    
    float damage;
    
};

Weapon LoadWeapon(const char* filePath, const char* weaponName);
void ReloadWeapon(Player& player);
void UpdateWeapon(Player& player);
