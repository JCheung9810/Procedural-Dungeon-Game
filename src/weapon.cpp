#include "weapon.h"

#include "json.hpp"

#include <fstream>
#include <cstring>
#include <string>

using json = nlohmann::json;

Weapon LoadWeapon(const char* filePath, const char* weaponName)
{
    Weapon weapon{};

    TraceLog(LOG_INFO, "Loading weapon: %s", weaponName);

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        TraceLog(LOG_ERROR, "Failed to open weapon config: %s", filePath);
        return weapon;
    }

    try
    {
        json data;
        file >> data;

        for (const auto& item : data.at("weapons"))
        {
            if (item.at("name").get<std::string>() == weaponName)
            {
                std::string name = item.at("name").get<std::string>();
                std::string type = item.at("type").get<std::string>();
                std::string path = item.at("texturePath").get<std::string>();

                weapon.name = strdup(name.c_str());
                weapon.type = strdup(type.c_str());
                weapon.texturePath = strdup(path.c_str());

                weapon.size = item.at("size").get<float>();
                weapon.positionOffset = item.at("positionOffset").get<float>();
                weapon.projectileOffset = item.at("projectileOffset").get<float>();

                weapon.texture = LoadTexture(weapon.texturePath);

                if (weapon.texture.id == 0)
                {
                    TraceLog(LOG_ERROR, "Failed to load weapon texture: %s", weapon.texturePath);
                }
                else
                {
                    TraceLog(LOG_INFO, "Loaded weapon: %s (Type: %s)", weapon.name, weapon.type);
                }

                return weapon;
            }
        }

        TraceLog(LOG_WARNING, "Weapon not found: %s", weaponName);
    }
    catch (const std::exception& e)
    {
        TraceLog(LOG_ERROR, "Error reading weapon JSON: %s", e.what());
    }

    return weapon;
}