#pragma once

#include "raylib.h"

#include "weapon.h"

#include <vector>

using namespace std;

struct Player;
struct Dungeon;
struct Projectile;

enum class EnemyMovementState {
    IDLE,
    WALKING,
    STUNNED
};

enum class EnemyAttackState {
    IDLE,
    ATTACKING,
    RELOADING,
    STUNNED

};

struct Enemy {
    const char* name;
    const char* type;
    
    Vector2 size;
    Vector2 position;
    Vector2 center;
    
    float health;
    float speed;

    float stunDuration;     //How much longer until the enemy gets out of stun (timer)
    //float iFrames
    
    Rectangle hitbox;

    Weapon weapon;

    EnemyMovementState movementState;
    EnemyAttackState attackState;
    
    float attackRate;       //How often entity can attack, at most  (what the timer is set to)
    float attackCD;         //How long before entity can attack again (timer)

    float minRange;         //How close until enemy starts moving away
    float maxRange;         //How far until enemy starts moving towards

    float aimAngle;

    float intelligence;
    
    bool active;
    Texture2D texture;
    
};

void AddEnemy(vector<Enemy>& enemies, const char* name, Vector2 position);
void UpdateEnemies(vector<Enemy>& enemies, Player& player, Dungeon& dungeon, vector<Projectile>& projectiles);
