#include "raylib.h" //C:\raylib\raylib\src\raylib.h
#include <raymath.h>
#include <string.h>

#include "player.h"
#include "dungeon.h"
#include "enemy.h"
#include "projectile.h"
#include "camera.h"
#include "draw.h"
#include "debug.h"
#include "assetManager.h"

#include <vector>

using namespace std;

//Assets
AssetManager assets;

//Debugging
Debug debug{};

//------------------------------MAIN------------------------------
int main(void){
    
    ChangeDirectory(GetApplicationDirectory());
    SetTraceLogLevel(LOG_DEBUG);
    
    //Initial screen size
    int screenWidth = 800;
    int screenHeight = 450;

    //Window creation
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Procedural Dungeon Game");
    MaximizeWindow();
    ClearWindowState(FLAG_WINDOW_RESIZABLE);
    //ToggleBorderlessWindowed();
    
    //Update screen size param with monitor resolution
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    
    //Audio
    InitAudioDevice();

    //FPS
    SetTargetFPS(60);

    //Disable escape key to close window
    SetExitKey(KEY_NULL);
    bool exitWindowRequested = false;
    bool exitWindow = false;
    
    //Player
    Player player;
    InitializePlayer(player);
    
    //Enemies
    vector<EnemyData> enemyData;
    vector<Enemy> enemies;

    LoadEnemyData(enemyData);
    
    AddEnemy(enemies, enemyData, "Dummy God", Vector2{300, -300});
    
    //Camera
    Camera2D camera = {};
    InitializeCamera(camera, player, screenHeight, screenWidth);
    
    //Mouse
    Vector2 mouseScreenPos = GetMousePosition();
    Vector2 mouseWorldPos;
    float playerToMouseRotation = 0.0f;
    float cursorSize = 60.0f;
    
    HideCursor();
    
    //Cursor Image
    Texture2D cursorTexture = assets.LoadGameTexture("assets/textures/UI/Cursor.png");
    
    //Images
    Texture2D heartTexture = assets.LoadGameTexture("assets/textures/UI/Heart.png");
    Texture2D weaponHudBoxTexture = assets.LoadGameTexture("assets/textures/UI/weaponHudBox.png");
    Texture2D weaponHudBoxBackgroundTexture = assets.LoadGameTexture("assets/textures/UI/weaponHudBoxBackground.png");
    assets.LoadGameTexture("assets/textures/environment/dungeonTileset.png");
    assets.LoadSpriteInfo();

    //Font
    int codepoints[] = { 0x221E };
    Font font = LoadFontEx(
        "assets/fonts/unifont-18.0.01.otf",
        32,
        codepoints,
        1
    );
    
    //Projectile
    vector<Projectile> projectiles;

    //Minimap
    bool minimap = false;
    float minimapScale = 10.0f;
    Vector2 minimapPos = player.center / minimapScale;

    //Pausing
    bool pause = false;
    
    //Dungeon
    int seed = GetRandomValue(100000, 999999);
    //seed = 100000;
    SetRandomSeed(seed);
    
    TraceLog(LOG_DEBUG, "Current Seed: %i",seed);
    
    Dungeon dungeon;
    InitializeDungeon("config/dungeon.json", dungeon);
    
    GenerateDungeon(dungeon);
    
    //------------------------------MAIN GAME LOOP----------------------------------------------------------------------------------
    while (!exitWindow){

        //Debug
        if(debug.active){
            if(IsKeyPressed(KEY_ZERO)){
                seed = GetRandomValue(100000, 999999);
                SetRandomSeed(seed);
                GenerateDungeon(dungeon);
            }
            if(IsKeyPressed(KEY_FIVE)){
                debug.hitbox = debug.hitbox ^ true;
            }
            if(IsKeyPressed(KEY_SIX)){
                debug.hurtbox = debug.hurtbox ^ true;
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
            if(IsKeyPressed(KEY_FOUR)){
                debug.vision = debug.vision ^ true;
            }
        }   

        if(IsKeyPressed(KEY_M) && !exitWindowRequested){
            minimap = minimap ^ true;
            pause = pause ^ true;
        }

        if(minimap && pause && !exitWindowRequested){
            if(IsKeyDown(KEY_D)){
                minimapPos.x += 4.0f;
            }
            if(IsKeyDown(KEY_A)){
                minimapPos.x -= 4.0f;
            }
            if(IsKeyDown(KEY_W)){
                minimapPos.y -= 4.0f;
            }
            if(IsKeyDown(KEY_S)){
                minimapPos.y += 4.0f;
            }

            float scroll = GetMouseWheelMove();

            if (scroll != 0.0f){
                minimapScale += -scroll * 0.1f;
            }

        }

        //Exit confirmation
        if(WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)){
            exitWindowRequested = true;
            pause = true;
        }
        
        if(!exitWindowRequested && player.health > 0.0f && !pause){
            //Update mouse
            mouseScreenPos = GetMousePosition();

            //------------------------------PLAYER------------------------------
            //Update player
            UpdatePlayer(player, dungeon);
            
            //Weapon swap
            if(IsKeyPressed(KEY_Q)){
                player.equippedWeaponIndex = (player.equippedWeaponIndex - 1);
                if(player.equippedWeaponIndex < 0){
                    player.equippedWeaponIndex = (int)player.weapons.size() - 1;
                }
            } else if(IsKeyPressed(KEY_E)){
                player.equippedWeaponIndex = (player.equippedWeaponIndex + 1) % (int)player.weapons.size();
            }
            
            //Weapon reload
            if(IsKeyPressed(KEY_R)){
                ReloadWeapon(player);
            }
            
            //Update weapon
            UpdateWeapon(player);
            
            //------------------------------ENEMY------------------------------
            //Update enemies
            UpdateEnemies(enemies, player, dungeon, projectiles);
            
            //------------------------------PROJECTILES------------------------------
            //Update mouse
            mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            playerToMouseRotation = atan2(mouseWorldPos.y - player.center.y, mouseWorldPos.x - player.center.x);    //RADIANS
            
            //Fire player projectile
            if(player.movementState != PlayerMovementState::DASHING){
                if(player.weapons[player.equippedWeaponIndex].fireMode == FireMode::SINGLE){
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && player.weapons[player.equippedWeaponIndex].mag >= 1 && 
                    player.weapons[player.equippedWeaponIndex].fireTimer <= 0.0f && 
                    player.weapons[player.equippedWeaponIndex].reloading == false)
                    {
                        FireProjectile(projectiles, &playerToMouseRotation, "Player", player, nullptr);
                    }
                } else if(player.weapons[player.equippedWeaponIndex].fireMode == FireMode::AUTOMATIC){
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && player.weapons[player.equippedWeaponIndex].mag >= 1 && 
                    player.weapons[player.equippedWeaponIndex].fireTimer <= 0.0f &&
                    player.weapons[player.equippedWeaponIndex].reloading == false)
                    {
                        FireProjectile(projectiles, &playerToMouseRotation, "Player", player, nullptr);
                    }
                }
            }
            
            //Update all projectiles
            UpdateProjectiles(projectiles, dungeon, enemies, player);

            //------------------------------CAMERA------------------------------      
            UpdateGameCamera(dungeon, player, camera);

            //------------------------------DUNGEON------------------------------
            UpdateDungeon(dungeon, enemyData, player, enemies);
            
        } else if (exitWindowRequested){ 
            if (IsKeyPressed(KEY_Y)){ 
                exitWindow = true;
                break;
            
            }else if (IsKeyPressed(KEY_N)){
                exitWindowRequested = false;
                pause = false;
            }
            
        } else if(!exitWindowRequested && player.health <= 0.0f){
            if(IsKeyPressed(KEY_SPACE)){
                //InitializePlayer(player, playerConfig);
                
                player.center = {0,0};
                player.position = {
                    player.center.x - (player.hurtbox.width/2.0f), 
                    player.center.y - (player.hurtbox.height/2.0f)
                };
                player.health = player.maxHealth;
                
            }
            
        }

        //------------------------------DRAW------------------------------
        DrawGame(camera, player, dungeon, enemies, projectiles,  
            heartTexture, weaponHudBoxTexture, weaponHudBoxBackgroundTexture, font,
            mouseScreenPos, playerToMouseRotation, cursorTexture, cursorSize,
            minimap, minimapPos, minimapScale,
            screenWidth, screenHeight,
            exitWindowRequested);
    }
    
    ShowCursor();
    
    assets.UnloadAll();

    CloseAudioDevice();

    CloseWindow();

    return 0;
}