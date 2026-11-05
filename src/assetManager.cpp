#include "assetManager.h"

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