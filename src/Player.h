#pragma once

#include "room.h"
#include "weapon.h"

#include "raylib.h"

enum class State {
    IDLE,
    WALKING,
    DASHING
};

struct Player {
    Vector2 position;
    Vector2 center;
    Vector2 direction;

    float speed;
    float maxHealth;
    float health;
    float iFrames;

    Rectangle hitBox;

    Vector2 tempDirection;
    int facingDirection;
    
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    
    int walkAnimationFrames;
    
    State state;
    
    int equippedWeaponIndex;
    int numWeapons;
    Weapon weapon[3];

    float dashTime;
    float dashTimer;
    float dashCD;
    
    int dashAnimationFrames;
    
    
    Texture2D texture;
    Texture2D dashTexture;
};

struct PlayerConfig {
    float maxHealth;
    float speed;   
    float dashTime;
    int walkAnimationFrames;
    int dashAnimationFrames;
    const char* texturePath;
    const char* dashTexturePath;
};

PlayerConfig LoadPlayerConfig(const char* filePath);

void InitializePlayer(Player& player, PlayerConfig playerConfig);
void UpdatePlayer(Player& player, Room rooms[], int& numRoomLocs, bool debugSpeed, bool debugWall);