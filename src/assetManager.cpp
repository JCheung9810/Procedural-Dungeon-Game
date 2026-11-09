#include "assetManager.h"

Vector2 AssetManager::CalcTileCoord(int col, int row, int borderSpacing, int spacing, int size){ 
    col -= 1;
    row -= 1;
    //border spacing + xCord with width + spacing between tiles
    float x = borderSpacing + (size * col) + (spacing * col);
    float y = borderSpacing + (size * row) + (spacing * row);
    
    return (Vector2){x,y};
}

void AssetManager::SetSpriteInfo(SpriteID id, int col, int row, int borderSpacing, int spacing, int size) {
    Vector2 position = CalcTileCoord(col, row, borderSpacing, spacing, size);

    spriteInfo[(int)id].textureSrc = {
        position.x,
        position.y,
        (float)size,
        (float)size
    };
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
    
    int borderSpacing;
    int spacing;
    float size;
    
    borderSpacing = 2;
    spacing = 1;
    size = 16.0f;
    
    //Top walls
    SetSpriteInfo(SmallWallTop, 4, 1, borderSpacing, spacing, size);
    SetSpriteInfo(MediumWallTop, 4, 1, borderSpacing, spacing, size);
    SetSpriteInfo(LargeWallTop, 4, 1, borderSpacing, spacing, size);

    //Bottom walls
    SetSpriteInfo(SmallWallBottom, 4, 2, borderSpacing, spacing, size);
    SetSpriteInfo(MediumWallBottom, 4, 2, borderSpacing, spacing, size);
    SetSpriteInfo(LargeWallBottom, 4, 2, borderSpacing, spacing, size);

    //Left walls
    SetSpriteInfo(SmallWallLeft, 3, 1, borderSpacing, spacing, size);
    SetSpriteInfo(MediumWallLeft, 3, 1, borderSpacing, spacing, size);
    SetSpriteInfo(LargeWallLeft, 3, 1, borderSpacing, spacing, size);

    //Right walls
    SetSpriteInfo(SmallWallRight, 3, 2, borderSpacing, spacing, size);
    SetSpriteInfo(MediumWallRight, 3, 2, borderSpacing, spacing, size);
    SetSpriteInfo(LargeWallRight, 3, 2, borderSpacing, spacing, size);
    
    //Inner corner walls
    SetSpriteInfo(InnerCornerWallTopLeft, 1, 1, borderSpacing, spacing, size);
    SetSpriteInfo(InnerCornerWallTopRight, 2, 1, borderSpacing, spacing, size);
    SetSpriteInfo(InnerCornerWallBottomLeft, 1, 2, borderSpacing, spacing, size);
    SetSpriteInfo(InnerCornerWallBottomRight, 2, 2, borderSpacing, spacing, size);
    
    //Outer corner walls
    SetSpriteInfo(OuterCornerWallTopLeft, 5, 1, borderSpacing, spacing, size);
    SetSpriteInfo(OuterCornerWallTopRight, 6, 1, borderSpacing, spacing, size);
    SetSpriteInfo(OuterCornerWallBottomLeft, 5, 2, borderSpacing, spacing, size);
    SetSpriteInfo(OuterCornerWallBottomRight, 6, 2, borderSpacing, spacing, size);
    
    //Walls
    SetSpriteInfo(SmallWall, 12, 1, borderSpacing, spacing, size);
    SetSpriteInfo(MediumWall, 12, 1, borderSpacing, spacing, size);
    SetSpriteInfo(LargeWall, 12, 1, borderSpacing, spacing, size);
    SetSpriteInfo(FitWall, 12, 1, borderSpacing, spacing, size);
}

SpriteID GetSpriteID(const string& name) {
    static const unordered_map<string, SpriteID> wallIDs = {
        {"SmallWallTop", SmallWallTop},
        {"SmallWallBottom", SmallWallBottom},
        {"SmallWallLeft", SmallWallLeft},
        {"SmallWallRight", SmallWallRight},

        {"MediumWallTop", MediumWallTop},
        {"MediumWallBottom", MediumWallBottom},
        {"MediumWallLeft", MediumWallLeft},
        {"MediumWallRight", MediumWallRight},

        {"LargeWallTop", LargeWallTop},
        {"LargeWallBottom", LargeWallBottom},
        {"LargeWallLeft", LargeWallLeft},
        {"LargeWallRight", LargeWallRight},

        {"InnerCornerWallTopLeft", InnerCornerWallTopLeft},
        {"InnerCornerWallTopRight", InnerCornerWallTopRight},
        {"InnerCornerWallBottomLeft", InnerCornerWallBottomLeft},
        {"InnerCornerWallBottomRight", InnerCornerWallBottomRight},

        {"OuterCornerWallTopLeft", OuterCornerWallTopLeft},
        {"OuterCornerWallTopRight", OuterCornerWallTopRight},
        {"OuterCornerWallBottomLeft", OuterCornerWallBottomLeft},
        {"OuterCornerWallBottomRight", OuterCornerWallBottomRight},
        {"SmallWall", SmallWall},
        {"MediumWall", MediumWall},
        {"LargeWall", LargeWall},
        {"FitWall", FitWall}
    };

    auto i = wallIDs.find(name);

    if (i == wallIDs.end()) {
        return SmallWallTop; // Fallback
    }

    return i->second;
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