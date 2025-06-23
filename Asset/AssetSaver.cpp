#include "pch.h"
#include "AssetSaver.h"

/*
PURPOSE: Saves json of the entity to a file which is given id
*/
void AssetSaver::SaveEntityToFile(nlohmann::json entityJson, const std::string& projectDir, const std::string& entityId) {
    std::string entitiesDir = projectDir + "entities/";
    std::filesystem::create_directory(entitiesDir);
    std::string entityDir = entitiesDir + entityId + "/";
    std::filesystem::create_directory(entityDir);
    std::string entityFile = entityDir + entityId + ".adrengineentity";

    std::ofstream file(entityFile);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving entity asset.");
        return;
    }

    file << entityJson.dump(4);
}

/*
PURPOSE: Loads json of the entity from a file which is given id
*/
nlohmann::json AssetSaver::LoadEntityFromFile(const std::string& projectDir, const std::string& entityId)
{
    std::string entitiesDir = projectDir + "entities/";
    std::string entityDir = entitiesDir + entityId + "/";
    std::string entityFile = entityDir + entityId + ".adrengineentity";

    std::ifstream file(entityFile);
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
void AssetSaver::SaveSceneToFile(nlohmann::json sceneJson, const std::string& projectDir, const std::string& sceneId)
{
    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string sceneDir = scenesDir + sceneId + "/";
    std::filesystem::create_directory(sceneDir);
    std::string sceneFile = sceneDir + sceneId + ".adrenginescene";

    std::ofstream file(sceneFile);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving scene asset.");
        return;
    }

    file << sceneJson.dump(4);
}

/*
PURPOSE: Loads json of the scene from a file which is given id
*/
nlohmann::json AssetSaver::LoadSceneFromFile(const std::string& projectDir, const std::string& sceneId)
{
    std::string scenesDir = projectDir + "scenes/";
    std::string sceneDir = scenesDir + sceneId + "/";
    std::string sceneFile = sceneDir + sceneId + ".adrenginescene";

    std::ifstream file(sceneFile);
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
void AssetSaver::SaveScriptToFile(nlohmann::json scriptJson, const std::string& projectDir, const std::string& scriptId)
{
    std::string scriptsDir = projectDir + "scripts/";
    std::filesystem::create_directory(scriptsDir);
    std::string scriptDir = scriptsDir + scriptId + "/";
    std::filesystem::create_directory(scriptDir);
    std::string scriptFile = scriptDir + scriptId + ".adrenginescript";

    std::ofstream file(scriptFile);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving script asset.");
        return;
    }

    file << scriptJson.dump(4);
}

/*
PURPOSE: Loads json of the script from a file which is given id
*/
nlohmann::json AssetSaver::LoadScriptFromFile(const std::string& projectDir, const std::string& scriptId)
{
    std::string scriptsDir = projectDir + "scripts/";
    std::string scriptDir = scriptsDir + scriptId + "/";
    std::string scriptFile = scriptDir + scriptId + ".adrenginescript";

    std::ifstream file(scriptFile);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading script asset.");
        return nullptr;
    }

    nlohmann::json jsonData;
    file >> jsonData;

    return jsonData;
}
