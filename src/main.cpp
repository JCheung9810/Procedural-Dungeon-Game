#include "raylib.h" //C:\raylib\raylib\src\raylib.h
#include <raymath.h>
#include <string.h>

#include "player.h"
#include "room.h"
#include "enemy.h"
#include "projectile.h"
#include "camera.h"
#include "draw.h"
#include "debug.h"

//------------------------------MAIN------------------------------
int main(void){
    
    ChangeDirectory(GetApplicationDirectory());
    
    //Initial screen size
    int screenWidth = 800;
    int screenHeight = 450;

    //Window creation
    InitWindow(screenWidth, screenHeight, "Procedural Dungeon Game");
    ToggleBorderlessWindowed();
    
    //Update screen size param with monitor resolution
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    //FPS
    SetTargetFPS(60);
    
    //Debugging
    Debug debug{};
    
    //Disable escape key to close window
    SetExitKey(KEY_NULL);
    bool exitWindowRequested = false;
    bool exitWindow = false;
    
    //Player
    Player player;
    PlayerConfig playerConfig = LoadPlayerConfig("config/player.json");
    InitializePlayer(player, playerConfig);
    
    //Enemies
    int numEnemies = 0;
    Enemy enemies[100] = {0};
    
    AddEnemy(enemies, "Dummy", numEnemies);
    
    //Camera
    Camera2D camera = {0};
    InitializeCamera(camera, player, screenHeight, screenWidth);
    
    //Mouse
    Vector2 mouseWorldPos;
    float playerToMouseRotation = 0.0f;
    float cursorSize = 60.0f;
    
    HideCursor();
    
    //Cursor Image
    Texture2D cursorTexture = LoadTexture("assets/Cursor.png");
    
    //Images
    Texture2D heartTexture = LoadTexture("assets/Heart.png");
    
    //Projectile
    float projectileLifespan = 5.0f;
    int maxProjectiles = 100;
    Projectile projectiles[maxProjectiles] = {0};
    
    InitializeProjectiles(projectiles, maxProjectiles);
    
    //Dungeon Rooms
    int seed = GetRandomValue(100000, 999999);
    //seed = 100000;
    SetRandomSeed(seed);
      
    TraceLog(LOG_INFO, "Current Seed: %i",seed);
    
    DungeonConfig dungeonConfig = LoadDungeonConfig("config/dungeon.json");
    
    int currentRooms = 0;
    int numRoomLocs = 0;
    
    Room rooms[500] = {};
    
    GenerateDungeon(rooms, dungeonConfig, currentRooms, numRoomLocs);
        
    //------------------------------MAIN GAME LOOP----------------------------------------------------------------------------------
    while (!exitWindow){
        
        if(!exitWindowRequested && player.health > 0.0f){
            //------------------------------PLAYER------------------------------
            //Update player
            UpdatePlayer(player, rooms, numRoomLocs, debug.speed, debug.wall);
            
            //Weapon swap
            if(IsKeyPressed(KEY_Q)){
                player.equippedWeaponIndex = (player.equippedWeaponIndex - 1);
                if(player.equippedWeaponIndex < 0){
                    player.equippedWeaponIndex = player.numWeapons - 1;
                }
            } else if(IsKeyPressed(KEY_E)){
                player.equippedWeaponIndex = (player.equippedWeaponIndex + 1) % player.numWeapons;
            }
            
            //Weapon reload
            if(IsKeyPressed(KEY_R)){
                ReloadWeapon(player);
            }
            
            //Update weapon
            UpdateWeapon(player);
            
            //------------------------------ENEMY------------------------------
            //Update enemies
            UpdateEnemies(enemies, numEnemies, player, rooms, numRoomLocs);
            
            //------------------------------PROJECTILES------------------------------
            //Update mouse
            mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            playerToMouseRotation = atan2(mouseWorldPos.y - player.center.y, mouseWorldPos.x - player.center.x);    //RADIANS
            
            //Fire player projectile
            if(player.weapon[player.equippedWeaponIndex].fireMode == FireMode::SINGLE){
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && player.weapon[player.equippedWeaponIndex].mag >= 1 && 
                player.weapon[player.equippedWeaponIndex].fireTimer <= 0.0f && 
                player.weapon[player.equippedWeaponIndex].reloading == false)
                {
                    FireProjectile(projectiles, maxProjectiles, playerToMouseRotation, "Player", projectileLifespan, player, enemies[0]);
                }
            } else if(player.weapon[player.equippedWeaponIndex].fireMode == FireMode::AUTOMATIC){
                if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && player.weapon[player.equippedWeaponIndex].mag >= 1 && 
                player.weapon[player.equippedWeaponIndex].fireTimer <= 0.0f &&
                player.weapon[player.equippedWeaponIndex].reloading == false)
                {
                    FireProjectile(projectiles, maxProjectiles, playerToMouseRotation, "Player", projectileLifespan, player, enemies[0]);
                }
            }
            
            //Fire test enemy projectiles     
            for(int i = 0; i < numEnemies; i++){
                enemies[i].attackCD -= GetFrameTime();
                if(enemies[i].attackCD <= 0.0f){
                    FireProjectile(projectiles, maxProjectiles, playerToMouseRotation, "Enemy", projectileLifespan, player, enemies[i]);
                    enemies[i].attackCD = enemies[i].attackRate;
                }
            }
            
            //Update all projectiles
            UpdateProjectiles(projectiles, maxProjectiles, rooms, numRoomLocs, enemies, numEnemies, player);

            //------------------------------CAMERA------------------------------      
            UpdateGameCamera(rooms, numRoomLocs, player, camera, debug.cam);
            
            //Debug
            if(debug.active){
                if(IsKeyPressed(KEY_ZERO)){
                    seed = GetRandomValue(100000, 999999);
                    SetRandomSeed(seed);
                    GenerateDungeon(rooms, dungeonConfig, currentRooms, numRoomLocs);
                }
                if(IsKeyPressed(KEY_SIX)){
                    debug.hitbox = debug.hitbox ^ true;
                }
                if(IsKeyPressed(KEY_SEVEN)){
                    debug.speed = debug.speed ^ true;
                }
                if(IsKeyPressed(KEY_EIGHT)){
                    debug.cam = debug.cam ^ true;
                }
                if(IsKeyPressed(KEY_NINE)){
                    debug.wall = debug.wall ^ true;
                }
            }   

            //Exit confirmation
            if(WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)){
                exitWindowRequested = true;
            }
            
        } else if (exitWindowRequested){ 
            if (IsKeyPressed(KEY_Y)){ 
                exitWindow = true;
                break;
            
            }else if (IsKeyPressed(KEY_N)){
                exitWindowRequested = false;
            }
            
        } else if(!exitWindowRequested && player.health <= 0.0f){
            if(IsKeyPressed(KEY_SPACE)){
                InitializePlayer(player, playerConfig);
                /*
                player.center = {0,0};
                player.position = {
                    player.center.x - (player.hitBox.width/2.0f), 
                    player.center.y - (player.hitBox.height/2.0f)
                };
                player.health = player.maxHealth;
                */
            }
            
        }

        //------------------------------DRAW------------------------------
        DrawGame(camera, player, rooms, numRoomLocs, enemies, numEnemies, projectiles, maxProjectiles,  
            heartTexture,
            playerToMouseRotation, cursorTexture, cursorSize, 
            screenWidth, screenHeight,
            debug.hitbox,
            exitWindowRequested);
    }
    
    ShowCursor();

    CloseWindow();

    return 0;
}