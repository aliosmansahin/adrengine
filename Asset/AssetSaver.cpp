#include "pch.h"
#include "AssetSaver.h"

/*
PURPOSE: Saves json of the entity to a file which is given id
*/
void AssetSaver::SaveEntityToFile(nlohmann::json entityJson, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving entity asset.");
        return;
    }

    file << entityJson.dump(4);
}

/*
PURPOSE: Loads json of the entity from a file which is given id
*/
nlohmann::json AssetSaver::LoadEntityFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading entity asset.");
        return nullptr;
    }

    nlohmann::json jsonData;
    file >> jsonData;

    return jsonData;
}

/*
PURPOSE: Saves json of the scene to a file which is given id
*/
void AssetSaver::SaveSceneToFile(nlohmann::json sceneJson, const std::string& filepath, std::string& projectDir)
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving scene asset.");
        return;
    }

    file << sceneJson.dump(4);
}

/*
PURPOSE: Loads json of the scene from a file which is given id
*/
nlohmann::json AssetSaver::LoadSceneFromFile(const std::string& filepath, std::string& projectDir)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading scene asset.");
        return nullptr;
    }

    nlohmann::json jsonData;
    file >> jsonData;



    return jsonData;
}

/*
PURPOSE: Saves json of the project to a file which is given id
*/
void AssetSaver::SaveProjectToFile(const std::string& filepath, nlohmann::json projectJson)
{
    std::ofstream file(filepath.c_str());
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving project asset.");
        return;
    }

    file << projectJson.dump(4);
}

/*
PURPOSE: Saves json of the script to a file which is given id
*/
void AssetSaver::SaveScriptToFile(nlohmann::json scriptJson, const std::string& filepath)
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving script asset.");
        return;
    }

    file << scriptJson.dump(4);
}

/*
PURPOSE: Loads json of the script from a file which is given id
*/
nlohmann::json AssetSaver::LoadScriptFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading script asset.");
        return nullptr;
    }

    nlohmann::json jsonData;
    file >> jsonData;

    return jsonData;
}
