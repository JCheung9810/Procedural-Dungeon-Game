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
    WALKING
};

enum class EnemyAttackState {
    IDLE,
    ATTACKING,
    RELOADING
};

enum class EnemyStatus {
    ALIVE,
    STUNNED,
    DEAD
};

struct Enemy {
    const char* name;
    const char* type;
    
    Vector2 size;
    Vector2 position;
    Vector2 center;

    Vector2 movementVector;
    
    float health;
    float speed;

    float stunDuration;     //How much longer until the enemy gets out of stun (timer)
    float stunTime;         //How long the enemy is stunned for (what the timer is set to)
    //float iFrames

    Vector2 knockbackDirection;
    float knockbackForce;
    
    Rectangle hitbox;

    Weapon weapon;

    EnemyMovementState movementState;
    EnemyAttackState attackState;
    EnemyStatus status;
    
    float attackRate;       //How often entity can attack, at most  (what the timer is set to)
    float attackCD;         //How long before entity can attack again (timer)

    float minRange;         //How close until enemy starts moving away
    float maxRange;         //How far until enemy starts moving towards

    float aimAngle;

    float intelligence;
    
    Texture2D texture;

    int walkAnimationFrames;
    int deathAnimationFrames;

    float frameTimer;               //Timer for changing frames
    float walkingFrameTime;         //What the timer is set to for changing walking frames
    float stunnedFrameTime;           //What the timer is set to for changing stunned frames
    int currentFrame;               //Current animation frame

    float despawnTime;      //What the time is set to for despawn time
    float despawnTimer;     //Timer for despawning
    
};

void AddEnemy(vector<Enemy>& enemies, const char* name, Vector2 position);
Vector2 KnockbackHandler(Enemy& enemy);
void UpdateEnemies(vector<Enemy>& enemies, Player& player, Dungeon& dungeon, vector<Projectile>& projectiles);
