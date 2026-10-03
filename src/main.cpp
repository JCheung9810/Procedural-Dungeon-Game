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
    
    //Enemy dummy
    int numEnemies = 0;
    Enemy enemies[30] = {0};
    
    AddEnemy(enemies, "Dummy", numEnemies);
    
    float projectileCD = 1.0f;
    
    //Camera
    Camera2D camera = {0};
    camera.target = player.center;
    camera.offset = (Vector2){screenWidth/2.0f, screenHeight/2.0f};
    camera.zoom = 1.0f;
    
    //Mouse
    Vector2 mouseWorldPos;
    float playerToMouseRotation;
    float cursorSize = 60.0f;
    
    HideCursor();
    
    //Cursor Image
    Texture2D cursorTexture = LoadTexture("../assets/Cursor.png");
    
    //Images
    Texture2D gunTexture = LoadTexture("../assets/Pistol.png");
    float gunSize = 80.0f;
    float gunOffset = 20.0f;
    char* gunType = (char*)"Pistol";    
    
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
    
    int currentRooms = 0;
    int maxRooms = 20;
    int numRoomLocs = 0;
    int roomSize = 1998;
    float wallDepth = 100;
    int doorSize = 250;
    
    float straight = 30;
    float turn = 30;
    float threeWay = 30;
    float fourWay = 10;  

    int bossRoomIndex;    
    
    Room rooms[500] = {};
    
    GenerateDungeon(rooms, currentRooms, maxRooms, numRoomLocs, roomSize, wallDepth, doorSize, straight, turn, threeWay, fourWay, bossRoomIndex);
        
    //------------------------------MAIN GAME LOOP----------------------------------------------------------------------------------
    while (!exitWindow){
        
        //------------------------------PLAYER------------------------------
        //Update player
        UpdatePlayer(player, rooms, numRoomLocs, debugSpeed, debugWall);

        
        //------------------------------PROJECTILES------------------------------
        //Update mouse
        mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        playerToMouseRotation = atan2(mouseWorldPos.y - player.center.y, mouseWorldPos.x - player.center.x);
        
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            FirePlayerProjectile(projectiles,maxProjectiles, playerToMouseRotation, gunType, gunSize, projectileLifespan, player, gunOffset);
        }
        
        UpdateProjectile(projectiles, maxProjectiles, rooms, numRoomLocs, enemies, numEnemies, player);
        
        
        //Test enemy projectile
        //Fire projectile       
        projectileCD -= GetFrameTime();
        if(projectileCD <= 0.0f){
            for (int i = 0; i < maxProjectiles; i++){
                if (!projectiles[i].active){
                    projectiles[i].active = true;
                    projectiles[i].lifeSpan = projectileLifespan; 
                    
                    projectiles[i].rotation = 180.0f * DEG2RAD;
                    projectiles[i].speed.x = cos(projectiles[i].rotation) * 1000;
                    projectiles[i].speed.y = sin(projectiles[i].rotation) * 1000;
                    
                    projectiles[i].position = (Vector2){enemies[0].position.x + enemies[0].size.x/2.0f, enemies[0].position.y + enemies[0].size.y/2.0f};
                    
                    projectiles[i].hitBox = {projectiles[i].position.x - projectiles[i].size.x/2.0f, projectiles[i].position.y  - projectiles[i].size.y/2.0f, projectiles[i].size.x, projectiles[i].size.y};
                    
                    projectiles[i].team = (char*)"Enemy";
                    projectiles[i].type = (char*)"Pistol";
                    projectileCD = 1.0f;
                    break;
                }
            }
        }
        
        

        //------------------------------CAMERA------------------------------      
        UpdateGameCamera(rooms, numRoomLocs, player, camera, debugCam);
        
        //Debug
        if(debugKeys){
            if(IsKeyPressed(KEY_ZERO)){
                seed = GetRandomValue(100000, 999999);
                SetRandomSeed(seed);
                GenerateDungeon(rooms, currentRooms, maxRooms, numRoomLocs, roomSize, wallDepth, doorSize, straight, turn, threeWay, fourWay, bossRoomIndex);
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
            gunTexture, gunSize, gunOffset, playerToMouseRotation, cursorTexture, cursorSize, screenWidth, screenHeight, exitWindowRequested);
    }
    
    ShowCursor();

    CloseWindow();

    return 0;
}