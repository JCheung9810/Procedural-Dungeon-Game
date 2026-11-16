#pragma once

#include "dungeon.h"
#include "weapon.h"
#include "assetManager.h"

#include <vector>

#include "raylib.h"

struct Dungeon;

enum class PlayerMovementState {
    IDLE,
    WALKING,
    DASHING
};

enum class PlayerAttackState {
    IDLE,
    ATTACKING,
    RELOADING
};

struct Player {
    Vector2 position;
    Vector2 center;
    Vector2 direction;

    float baseSpeed;
    float currentSpeed;
    float maxHealth;
    float health;
    float iFrames;

    Vector2 size;

    Rectangle hitbox;
    Rectangle hurtbox;

    Vector2 tempDirection;
    int facingDirection;
    
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    
    int walkAnimationFrames;
    
    PlayerMovementState movementState;
    PlayerAttackState attackState;
    
    int equippedWeaponIndex;
    vector<Weapon> weapons;
    int maxWeapons;

    float dashDistance;         //Distance of dash
    float dashTime;             //Duration of dash
    float dashTimer;            //Timer for duration of dash

    float dashCDTimer;          //Timer for dash cooldown
    float dashCD;               //Cooldown amount for dashing
    
    int dashAnimationFrames;
    
    
    Texture2D texture;

    Texture2D dashTexture;
};

void InitializePlayer(Player& player);
void UpdatePlayer(Player& player, Dungeon& dungeon, bool debugSpeed, bool debugWall);
