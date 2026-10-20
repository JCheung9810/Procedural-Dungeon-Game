#include "raylib.h" //C:\raylib\raylib\src\raylib.h
#include <raymath.h>
#include <string.h>

#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Camera.h"
#include "Draw.h"

//------------------------------MAIN------------------------------
int main(void){
    
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
    bool debugKeys = true;
    
    bool debugSpeed = false;
    bool debugWall = false;
    bool debugCam = false;
    
    //Disable escape key to close window
    SetExitKey(KEY_NULL);
    bool exitWindowRequested = false;
    bool exitWindow = false;
    
    //Player
    Player player;
    InitializePlayer(player);
    
    //Enemies
    int numEnemies = 0;
    Enemy enemies[100] = {0};
    
    AddEnemy(enemies, (char*)"Dummy", numEnemies);
    
    //Camera
    Camera2D camera = {0};
    InitializeCamera(camera, player, screenHeight, screenWidth);
    
    //Mouse
    Vector2 mouseWorldPos;
    float playerToMouseRotation;
    float cursorSize = 60.0f;
    
    HideCursor();
    
    //Cursor Image
    Texture2D cursorTexture = LoadTexture("../assets/Cursor.png");
    
    //Images
    Texture2D heartTexture = LoadTexture("../assets/Heart.png");
    
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
    
    DungeonConfig config;
    
    config.maxRooms = 20;
    config.roomSize = 1998;
    config.wallDepth = 100;
    config.doorSize = 250;

    config.straight = 30;
    config.turn = 30;
    config.threeWay = 30;
    config.fourWay = 10;
    
    int currentRooms = 0;
    int numRoomLocs = 0;
    
    Room rooms[500] = {};
    
    GenerateDungeon(rooms, config, currentRooms, numRoomLocs);
        
    //------------------------------MAIN GAME LOOP----------------------------------------------------------------------------------
    while (!exitWindow){
        
        //------------------------------PLAYER------------------------------
        //Update player
        UpdatePlayer(player, rooms, numRoomLocs, debugSpeed, debugWall);
        
        //------------------------------ENEMY------------------------------
        //Update enemies
        UpdateEnemies(enemies, numEnemies, player, rooms, numRoomLocs);
        
        //------------------------------PROJECTILES------------------------------
        //Update mouse
        mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        playerToMouseRotation = atan2(mouseWorldPos.y - player.center.y, mouseWorldPos.x - player.center.x);
        
        //Fire player projectile
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            FireProjectile(projectiles,maxProjectiles, playerToMouseRotation, (char*)"Player", player.gunType, player.gunSize, projectileLifespan, player, enemies[0], player.gunOffset);
        }
        
        //Fire test enemy projectile     
        for(int i = 0; i < numEnemies; i++){
            enemies[i].attackCD -= GetFrameTime();
            if(enemies[i].attackCD <= 0.0f){
                FireProjectile(projectiles,maxProjectiles, playerToMouseRotation, (char*)"Enemy", player.gunType, player.gunSize, projectileLifespan, player, enemies[i], player.gunOffset);
                enemies[i].attackCD = enemies[i].attackRate;
            }
        }
        
        UpdateProjectiles(projectiles, maxProjectiles, rooms, numRoomLocs, enemies, numEnemies, player);

        //------------------------------CAMERA------------------------------      
        UpdateGameCamera(rooms, numRoomLocs, player, camera, debugCam);
        
        //Debug
        if(debugKeys){
            if(IsKeyPressed(KEY_ZERO)){
                seed = GetRandomValue(100000, 999999);
                SetRandomSeed(seed);
                GenerateDungeon(rooms, config, currentRooms, numRoomLocs);
            }
            if(IsKeyPressed(KEY_SEVEN)){
                debugSpeed = debugSpeed ^ true;
            }
            if(IsKeyPressed(KEY_EIGHT)){
                debugCam = debugCam ^ true;
            }
            if(IsKeyPressed(KEY_NINE)){
                debugWall = debugWall ^ true;
            }
        }   

        //Exit confirmation
        if(WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) 
            exitWindowRequested = true;
        
        if (exitWindowRequested){
            if (IsKeyPressed(KEY_Y)){ 
                exitWindow = true;
                break;
            
            }else if (IsKeyPressed(KEY_N)) 
                exitWindowRequested = false;
        }

        //------------------------------DRAW------------------------------
        DrawGame(camera, player, rooms, numRoomLocs, enemies, numEnemies, projectiles, maxProjectiles, 
            player.gunTexture, player.gunSize, player.gunOffset, 
            heartTexture,
            playerToMouseRotation, cursorTexture, cursorSize, 
            screenWidth, screenHeight, exitWindowRequested);
    }
    
    ShowCursor();

    CloseWindow();

    return 0;
}