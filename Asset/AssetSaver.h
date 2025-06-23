#pragma once

#ifdef ASSET_EXPORTS
#define ASSET_API __declspec(dllexport)
#else
#define ASSET_API __declspec(dllimport)
#endif

#include <memory>
#include <fstream>

#include "nlohmann_json/json.hpp"
#include "Logger.h"

class ASSET_API AssetSaver {
public:
    //entity functions
    static void           SaveEntityToFile(nlohmann::json entityJson, const std::string& projectDir, const std::string& entityId);
    static nlohmann::json LoadEntityFromFile(const std::string& projectDir, const std::string& entityId);

    //scene functions
    static void           SaveSceneToFile(nlohmann::json sceneJson, const std::string& projectDir, const std::string& sceneId);
    static nlohmann::json LoadSceneFromFile(const std::string& projectDir, const std::string& sceneId);
    
    //project function(s)
    static void           SaveProjectToFile(const std::string& filepath, nlohmann::json projectJson);

    //script functions
    static void           SaveScriptToFile(nlohmann::json scriptJson, const std::string& projectDir, const std::string& scriptId);
    static nlohmann::json LoadScriptFromFile(const std::string& projectDir, const std::string& scriptId);
};
