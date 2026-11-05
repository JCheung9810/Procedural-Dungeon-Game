#include "assetManager.h"

#include <vector>

using namespace std;

static vector<Texture2D> textures;
static vector<Sound> sounds;

Texture2D AssetManager::LoadGameTexture(const char* path) {
    Texture2D texture = LoadTexture(path);

    if (texture.id != 0) {
        textures.push_back(texture);
    }

    return texture;
}

Sound AssetManager::LoadGameSound(const char* path) {
    Sound sound = LoadSound(path);

    if (sound.frameCount > 0) {
        sounds.push_back(sound);
    }

    return sound;
}

void AssetManager::UnloadAll() {
    for (Texture2D texture : textures) {
        UnloadTexture(texture);
    }

    for (Sound sound : sounds) {
        UnloadSound(sound);
    }

    textures.clear();
    sounds.clear();
}