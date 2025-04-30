#pragma once

#include <memory>
#include <fstream>

#include "nlohmann_json/json.hpp"
#include "Logger.h"

#include "Scene.h"

class Scene;

class AssetSaver {
public:
    static void SaveEntityToFile(Entity* entity, const std::string& filepath);
    static std::shared_ptr<Entity> LoadEntityFromFile(const std::string& filepath);
    static void SaveSceneToFile(Scene* scene, const std::string& filepath);
    static Scene* LoadSceneFromFile(const std::string& filepath);
    static void SaveProjectToFile(const std::string& filepath);
    static void SaveScriptToFile(VisualScript* script, const std::string& filepath);
    static VisualScript* LoadScriptFromFile(const std::string& filepath);
};
