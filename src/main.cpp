#include "raylib.h" //C:\raylib\raylib\src\raylib.h
#include <raymath.h>
#include <string.h>

#include "Player.h"
#include "Room.h"

//------------------------------STRUCTS------------------------------
typedef struct Projectile {
    Vector2 position;
    Vector2 speed;
    
    Vector2 size;
    float rotation;
    
    float lifeSpan;
    
    bool active;
    
    char* team;
    char* type;
    
    Color color;
    Texture2D sprite;
    
    Rectangle hitBox;
    //add 8 circles for detection so it can be "rotated" (4 corners 4 sides)
    //detect collision with the array of corners
} Projectile;

typedef struct Enemy {
    char* name;
    
    Vector2 size;
    Vector2 position;
    
    float health;
    
    Rectangle hitBox;
    
    bool active;
    Texture2D sprite;
    
} Enemy;

void AddEnemy(Enemy enemies[], char* name, int& numEnemies){
    if(TextIsEqual(name, "Dummy")){
        enemies[numEnemies].name = (char*)"Dummy";
        enemies[numEnemies].position = (Vector2){300,-300};
        enemies[numEnemies].health = 100.0f;
        enemies[numEnemies].size = (Vector2){40.0f,40.0f};
        
        enemies[numEnemies].hitBox = {enemies[numEnemies].position.x, enemies[numEnemies].position.y, enemies[numEnemies].size.x, enemies[numEnemies].size.y};
        Vector2 dummyCenter = (Vector2){enemies[numEnemies].position.x + enemies[numEnemies].size.x/2 ,enemies[numEnemies].position.y + enemies[numEnemies].size.y/2};
        
        enemies[numEnemies].active = true;
    }
    
    numEnemies++;
}

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
    Vector2 mouseScreenPos = GetMousePosition();
    float playerToMouseRotation;
    float cursorSize = 60.0f;
    Rectangle cursorRec;
    Rectangle cursorDestination;
    Vector2 cursorOrigin;
    
    HideCursor();
    
    //Cursor Image
    Texture2D cursorTexture = LoadTexture("../assets/Cursor.png");
    
    //Images
    Texture2D gunTexture = LoadTexture("../assets/Pistol.png");
    float gunSize = 80.0f;
    float gunOffset = 20.0f;
    char* gunType = (char*)"Pistol";    
    Vector2 gunPos;
    Rectangle gunRec;
    Rectangle gunDest;
    float gunAngle;
    
    Texture2D heartTexture = LoadTexture("../assets/Heart.png");
    
    //Projectile
    float projectileLifespan = 5.0f;
    int maxProjectiles = 100;
    Projectile projectile[maxProjectiles] = {0};
    
    for(int i = 0; i < maxProjectiles; i++){
        projectile[i].position = (Vector2){0,0};
        projectile[i].speed = (Vector2){0,0};
        projectile[i].size = {8,8};
        projectile[i].active = false;
        projectile[i].lifeSpan = 0.0f;
        projectile[i].color = RED;
        projectile[i].hitBox = {projectile[i].position.x, projectile[i].position.y, projectile[i].size.x, projectile[i].size.y};
        
    }    
    
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
        
        UpdatePlayer(player, rooms, numRoomLocs, debugSpeed, debugWall);

        
        //------------------------------PROJECTILES------------------------------
        //Update mouse
        mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        playerToMouseRotation = atan2(mouseWorldPos.y - player.center.y, mouseWorldPos.x - player.center.x);
        
        //Fire projectile       
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            for (int i = 0; i < maxProjectiles; i++){
                if (!projectile[i].active){
                    projectile[i].active = true;
                    projectile[i].lifeSpan = projectileLifespan; 
                    
                    projectile[i].rotation = playerToMouseRotation;
                    projectile[i].speed.x = cos(projectile[i].rotation) * 1000;
                    projectile[i].speed.y = sin(projectile[i].rotation) * 1000;
                    
                    if(TextIsEqual(gunType, "Pistol")){
                        projectile[i].position = (Vector2){player.center.x + cos(projectile[i].rotation) * gunSize - (cos(projectile[i].rotation) * gunOffset*2.3f), player.center.y + sin(projectile[i].rotation) * gunSize - (sin(projectile[i].rotation) * gunOffset*2.3f)};
                    }
                    
                    projectile[i].hitBox = {projectile[i].position.x - projectile[i].size.x/2.0f, projectile[i].position.y  - projectile[i].size.y/2.0f, projectile[i].size.x, projectile[i].size.y};
                    
                    projectile[i].team = (char*)"Player";
                    projectile[i].type = (char*)"Pistol";
                    break;
                }
            }
        }
        
        //Translate projectile
        for(int i = 0; i < maxProjectiles; i++){
            if(projectile[i].active){
                
                projectile[i].position = {
                    projectile[i].position.x + projectile[i].speed.x * GetFrameTime(), 
                    projectile[i].position.y + projectile[i].speed.y * GetFrameTime()
                };    

                projectile[i].hitBox = {projectile[i].position.x - projectile[i].size.x/2.0f, projectile[i].position.y  - projectile[i].size.y/2.0f, projectile[i].size.x, projectile[i].size.y};
                
            }
        }
        
        //Projectile collision/despawn
        for(int i = 0; i < maxProjectiles; i++){
            if(projectile[i].active){
                //Check walls
                for(int j = 0; j < numRoomLocs; j++){
                    if(rooms[j].exists == true){
                        for(int k = 0; k < rooms[j].numWalls; k++){
                            if(CheckCollisionRecs(rooms[j].walls[k],projectile[i].hitBox)){
                                projectile[i].active = false;
                                projectile[i].lifeSpan = 0.0f;
                            }
                        }
                    }
                }
                
                //Check enemy collision
                for(int j = 0; j < numEnemies; j++){
                    if(enemies[j].active == true && CheckCollisionRecs(enemies[j].hitBox,projectile[i].hitBox) && TextIsEqual(projectile[i].team, "Player")){
                        projectile[i].active = false;
                        projectile[i].lifeSpan = 0.0f;
                        
                        enemies[j].health -= 10.0f;
                        if(enemies[j].health <= 0.0f){
                            enemies[j].active = false;
                        }
                    }
                }
                
                //Check player collision
                if(CheckCollisionRecs(player.hitBox,projectile[i].hitBox) && TextIsEqual(projectile[i].team, "Enemy") && player.iFrames <= 0.0f){
                    projectile[i].active = false;
                    projectile[i].lifeSpan = 0.0f;
                    
                    player.health -= 10.0f;
                    player.iFrames = 1.0f;
                }
                
                projectile[i].lifeSpan -= GetFrameTime(); 
               
                if(projectile[i].lifeSpan <= 0.0f){
                   projectile[i].active = false;
                }
            }                    
        }
        
        //Player iFrames
        player.iFrames -= GetFrameTime();
        
        
        //Test enemy projectile
        //Fire projectile       
        projectileCD -= GetFrameTime();
        if(projectileCD <= 0.0f){
            for (int i = 0; i < maxProjectiles; i++){
                if (!projectile[i].active){
                    projectile[i].active = true;
                    projectile[i].lifeSpan = projectileLifespan; 
                    
                    projectile[i].rotation = 180.0f * DEG2RAD;
                    projectile[i].speed.x = cos(projectile[i].rotation) * 1000;
                    projectile[i].speed.y = sin(projectile[i].rotation) * 1000;
                    
                    projectile[i].position = (Vector2){enemies[0].position.x + enemies[0].size.x/2.0f, enemies[0].position.y + enemies[0].size.y/2.0f};
                    
                    projectile[i].hitBox = {projectile[i].position.x - projectile[i].size.x/2.0f, projectile[i].position.y  - projectile[i].size.y/2.0f, projectile[i].size.x, projectile[i].size.y};
                    
                    projectile[i].team = (char*)"Enemy";
                    projectile[i].type = (char*)"Pistol";
                    projectileCD = 1.0f;
                    break;
                }
            }
        }
        
        

        //------------------------------CAMERA------------------------------
        //Camera
        int playerRoom = -1;

        //Find the room that the player is in
        for (int i = 0; i < numRoomLocs; i++){
            if (rooms[i].exists){
                Rectangle roomRect = {
                    rooms[i].position.x,
                    rooms[i].position.y,
                    rooms[i].size.x,
                    rooms[i].size.y
                };

                if (CheckCollisionPointRec(player.center, roomRect)){
                    playerRoom = i;
                    break;
                }
            }
        }
        
        //If found
        if (playerRoom != -1){
            float roomLeft = rooms[playerRoom].position.x;
            float roomRight = roomLeft + rooms[playerRoom].size.x;

            float roomTop = rooms[playerRoom].position.y;
            float roomBottom = roomTop + rooms[playerRoom].size.y;

            float halfCameraWidth = GetScreenWidth() / (2.0f * camera.zoom);

            float halfCameraHeight = GetScreenHeight() / (2.0f * camera.zoom);

            //Clamp
            camera.target.x = Clamp(
                player.center.x,
                roomLeft + halfCameraWidth,
                roomRight - halfCameraWidth
            );

            camera.target.y = Clamp(
                player.center.y,
                roomTop + halfCameraHeight,
                roomBottom - halfCameraHeight
            );
        }    

        bool isBoss = false;
        if(playerRoom != -1){
            isBoss =
            (TextIsEqual(rooms[playerRoom].type, "BossN") ||
             TextIsEqual(rooms[playerRoom].type, "BossE") ||
             TextIsEqual(rooms[playerRoom].type, "BossS") ||
             TextIsEqual(rooms[playerRoom].type, "BossW"));
        }

        //Camera mode
        if(debugCam){
            camera.target = player.center;
            camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));
        } else {
            if(!isBoss){
                camera.zoom = 1.5f; 
            } else {
                camera.zoom = 0.5f;
            }                
        }
        
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
        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (!exitWindowRequested){
                
                BeginMode2D(camera);
                
                    int fontSize;
                    int textWidth;
                
                    //Draw rooms
                    for(int i = 0; i < numRoomLocs; i++){
                        if(rooms[i].exists == true){
                            //Draw doors
                            for(int j = 0; j < rooms[i].numDoors; j++){
                                DrawRectangleRec(rooms[i].doors[j], MAROON);                                 
                            }
                            //Draw walls
                            for(int j = 0; j < rooms[i].numWalls; j++){
                                DrawRectangleRec(rooms[i].walls[j], BLACK);                                 
                            }
                        }
                        fontSize = 30;
                        //Text Type
                        textWidth = MeasureText(rooms[i].type, fontSize);
                        DrawText(
                            rooms[i].type, rooms[i].position.x + (Vector2){rooms[i].size.x/2.0f,0}.x - textWidth/2, 
                            rooms[i].position.y + (Vector2){0,rooms[i].size.y/2.0f}.y - fontSize/2, 
                            fontSize, 
                            BLACK
                        );                       
                        //Text Dist
                        textWidth = MeasureText(TextFormat("Dist: %i", rooms[i].distance), fontSize);
                        DrawText(
                            TextFormat("Dist: %i", rooms[i].distance), rooms[i].position.x + (Vector2){rooms[i].size.x/2.0f,0}.x - textWidth/2, 
                            rooms[i].position.y + (Vector2){0,rooms[i].size.y/2.0f}.y - fontSize/2 + fontSize, 
                            fontSize, 
                            BLACK
                        ); 
                        //Text Index
                        textWidth = MeasureText(TextFormat("Index: %i", i), fontSize);
                        DrawText(
                            TextFormat("Index: %i", i), rooms[i].position.x + (Vector2){rooms[i].size.x/2.0f,0}.x - textWidth/2, 
                            rooms[i].position.y + (Vector2){0,rooms[i].size.y/2.0f}.y - fontSize/2 + fontSize * 2, 
                            fontSize, 
                            BLACK
                        );   
                        //Text Grid Position
                        textWidth = MeasureText(TextFormat("Grid Position: {%f,%f}", rooms[i].gridPos.x, rooms[i].gridPos.y), fontSize);
                        DrawText(
                            TextFormat("Grid Position: {%f,%f}", rooms[i].gridPos.x, rooms[i].gridPos.y), rooms[i].position.x + (Vector2){rooms[i].size.x/2.0f,0}.x - textWidth/2, 
                            rooms[i].position.y + (Vector2){0,rooms[i].size.y/2.0f}.y - fontSize/2 + fontSize * 3, 
                            fontSize, 
                            BLACK
                        ); 
                    }

                    DrawPlayer(player);
                    
                    //Draw dummy
                    for (int i = 0; i < numEnemies; i++){
                        if (enemies[i].active){
                            fontSize = 15;
                            textWidth = MeasureText(enemies[i].name, fontSize);
                            DrawText(
                                enemies[i].name, enemies[i].position.x + enemies[i].size.x/2.0f - textWidth/2, 
                                enemies[i].position.y - fontSize*2.2f, 
                                fontSize, 
                                BLACK
                            );   
                            textWidth = MeasureText(TextFormat("%0.2f",enemies[i].health), fontSize);
                            DrawText(
                                TextFormat("%0.2f",enemies[i].health), enemies[i].position.x + enemies[i].size.x/2.0f - textWidth/2, 
                                enemies[i].position.y - fontSize*1.1f, 
                                fontSize, 
                                BLACK
                            );   
                            DrawRectangleRec(enemies[i].hitBox, RED);
                        }
                    }
                    
                    //Draw projectiles
                    for (int i = 0; i < maxProjectiles; i++){
                        if (projectile[i].active) 
                            //DrawRectanglePro(projectile[i].hitBox, (Vector2){projectile[i].size.x / 2.0f, projectile[i].size.y / 2.0f}, projectile[i].rotation * RAD2DEG, projectile[i].color);
                            DrawRectangleRec(projectile[i].hitBox, YELLOW);
                    } 
                    
                    //Draw gun
                    gunPos = {player.center.x - cos(playerToMouseRotation) * gunOffset,player.center.y - sin(playerToMouseRotation) * gunOffset};
                    gunRec = {0,0,(float)gunTexture.width / 6.0f,(float)gunTexture.height - 30.0f};
                    gunDest = {gunPos.x,gunPos.y,gunSize,gunSize};
                    gunAngle = playerToMouseRotation * RAD2DEG;
                    
                    if (gunAngle < -90 || gunAngle > 90){
                        gunRec.height = -gunRec.height;
                    }
                    

                    DrawTexturePro(gunTexture, gunRec, gunDest, (Vector2){0, gunDest.height / 2.0f}, gunAngle, WHITE);
                    //              texture     source      dest               origin/pivot             rotation  color
              
                EndMode2D();
                
                mouseScreenPos = GetMousePosition();
                cursorRec = {0, 0, (float)cursorTexture.width, (float)cursorTexture.height};
                cursorDestination = {mouseScreenPos.x + (float)cursorTexture.width/2.0f - cursorSize/2.0f, mouseScreenPos.y + (float)cursorTexture.height/2.0f - cursorSize/2.0f, cursorSize, cursorSize};

                cursorOrigin = {cursorTexture.width / 2.0f, cursorTexture.height / 2.0f};

                DrawTexturePro(cursorTexture, cursorRec, cursorDestination, cursorOrigin, 0.0f, WHITE);
            } else {  
            
                //Exit menu
                DrawRectangle(0, screenHeight/2 - 100, screenWidth, 200, BLACK);
                int fontSize = 30;
                int textWidth = MeasureText("Are you sure you want to exit program? [Y/N]", fontSize);
                DrawText("Are you sure you want to exit program? [Y/N]", screenWidth/2 - textWidth/2, screenHeight/2 - fontSize/2, 30, WHITE);
            
            }
        EndDrawing();
    }
    
    ShowCursor();

    CloseWindow();

    return 0;
}