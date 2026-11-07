#include "assetManager.h"

Vector2 AssetManager::CalcTileCoord(int col, int row, int borderSpacing, int spacing, int size){
    float x,y;
    
    //border spacing + xCord with width + spacing between tiles
    x = (borderSpacing * row) + (size * (col - spacing)) + (col - spacing);
    y = (borderSpacing * col) + (size * (row - spacing)) + (row - spacing);
    
    return (Vector2){x,y};
}

Texture2D AssetManager::LoadGameTexture(const char* path) {
    //Check if texture is already loaded
    auto it = textures.find(path);

    if (it != textures.end()) {
        return it->second;
    }

    //Load texture
    Texture2D texture = LoadTexture(path);

    if (texture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load texture: %s", path);
        return {};
    }

    //Store texture in cache
    textures[path] = texture;

    TraceLog(LOG_INFO, "Loaded texture: %s", path);

    return texture;
}

Sound AssetManager::LoadGameSound(const char* path) {
    //Check if sound is already loaded
    auto it = sounds.find(path);

    if (it != sounds.end()) {
        return it->second;
    }

    //Load sound
    Sound sound = LoadSound(path);

    if (sound.frameCount == 0) {
        TraceLog(LOG_ERROR, "Failed to load sound: %s", path);
        return {};
    }

    //Store sound in cache
    sounds[path] = sound;

    TraceLog(LOG_INFO, "Loaded sound: %s", path);

    return sound;
}

void AssetManager::LoadSpriteInfo(){
    
    /*
    SmallWallTop,
    SmallWallBottom,
    SmallWallLeft,
    SmallWallRight,
    MediumWallTop,
    MediumWallBottom,
    MediumWallLeft,
    MediumWallRight,
    LargeWallTop,
    LargeWallBottom,
    LargeWallLeft,
    LargeWallRight
    */
    
    Vector2 position;
    int borderSpacing, spacing;
    float size;
    
    borderSpacing = 2;
    spacing = 1;
    size = 16.0f;
    
    //Top walls
    position = CalcTileCoord(4,1, borderSpacing, spacing, size);
    spriteInfo[SmallWallTop].textureSrc = {position.x, position.y, size, size};
    spriteInfo[MediumWallTop].textureSrc = {position.x, position.y, size, size};
    spriteInfo[LargeWallTop].textureSrc = {position.x, position.y, size, size};
    
    //Bottom walls
    position = CalcTileCoord(4,2, borderSpacing, spacing, size);
    spriteInfo[SmallWallBottom].textureSrc = {position.x, position.y, size, size};
    spriteInfo[MediumWallBottom].textureSrc = {position.x, position.y, size, size};
    spriteInfo[LargeWallBottom].textureSrc = {position.x, position.y, size, size};
    
    //Left walls
    position = CalcTileCoord(3,1, borderSpacing, spacing, size);
    spriteInfo[SmallWallLeft].textureSrc = {position.x, position.y, size, size};
    spriteInfo[MediumWallLeft].textureSrc = {position.x, position.y, size, size};
    spriteInfo[LargeWallLeft].textureSrc = {position.x, position.y, size, size};
    
    //Right walls
    position = CalcTileCoord(3,2, borderSpacing, spacing, size);
    spriteInfo[SmallWallRight].textureSrc = {position.x, position.y, size, size};
    spriteInfo[MediumWallRight].textureSrc = {position.x, position.y, size, size};
    spriteInfo[LargeWallRight].textureSrc = {position.x, position.y, size, size};
}

void AssetManager::UnloadAll() {
    //Unload textures
    for (auto& [path, texture] : textures) {
        UnloadTexture(texture);
    }

    //Unload sounds
    for (auto& [path, sound] : sounds) {
        UnloadSound(sound);
    }

    textures.clear();
    sounds.clear();

    TraceLog(LOG_INFO, "All assets unloaded");
}