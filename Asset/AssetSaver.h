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
    static void           SaveEntityToFile(nlohmann::json entityJson, const std::string& filepath);
    static nlohmann::json LoadEntityFromFile(const std::string& filepath);

    //scene functions
    static void           SaveSceneToFile(nlohmann::json sceneJson, const std::string& filepath, std::string& projectDir);
    static nlohmann::json LoadSceneFromFile(const std::string& filepath, std::string& projectDir);
    
    //project function(s)
    static void           SaveProjectToFile(const std::string& filepath, nlohmann::json projectJson);

    //script functions
    static void           SaveScriptToFile(nlohmann::json scriptJson, const std::string& filepath);
    static nlohmann::json LoadScriptFromFile(const std::string& filepath);
};
