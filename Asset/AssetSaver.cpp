#include "pch.h"
#include "AssetSaver.h"

/*
PURPOSE: Saves json of the entity to a file which is given id
*/
void AssetSaver::SaveEntityToFile(nlohmann::json entityJson, const std::string& projectDir, const std::string& entityId) {
    std::string entityFile = CreateTargetDirectories(projectDir, entityId, "entities", "adrengineentity");

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
    std::string entityFile = GetTargetFilePath(projectDir, entityId, "entities", "adrengineentity");

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
    std::string sceneFile = CreateTargetDirectories(projectDir, sceneId, "scenes", "adrenginescene");

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
    std::string sceneFile = GetTargetFilePath(projectDir, sceneId, "scenes", "adrenginescene");

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
    std::string scriptFile = CreateTargetDirectories(projectDir, scriptId, "scripts", "adrenginescript");

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
    std::string scriptFile = GetTargetFilePath(projectDir, scriptId, "scripts", "adrenginescript");

    std::ifstream file(scriptFile);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading script asset.");
        return nullptr;
    }

    nlohmann::json jsonData;
    file >> jsonData;

    return jsonData;
}

/*
PURPOSE: Creates directories for the target

    projectDir: Root directory for the project
    id: Target id
    folderName: A name for the container folder
    fileSuffix: file suffix for the target ("." will be added internally) 
    
    Return: The full path for the target file
*/
std::string AssetSaver::CreateTargetDirectories(const std::string& projectDir, const std::string& id, const std::string& folderName, const std::string& fileSuffix)
{
    std::string containerFolder = projectDir + folderName + "/";
    std::filesystem::create_directory(containerFolder);
    std::string targetDir = containerFolder + id + "/";
    std::filesystem::create_directory(targetDir);
    std::string targetFile = targetDir + id + "." + fileSuffix;

    return targetFile;
}

/*
PURPOSE: Gets full path for the target

    projectDir: Root directory for the project
    id: Target id
    folderName: A name for the container folder
    fileSuffix: file suffix for the target ("." will be added internally)

    Return: The full path for the target file
*/
std::string AssetSaver::GetTargetFilePath(const std::string& projectDir, const std::string& id, const std::string& folderName, const std::string& fileSuffix)
{
    std::string containerFolder = projectDir + folderName + "/";
    std::string targetDir = containerFolder + id + "/";
    std::string targetFile = targetDir + id + "." + fileSuffix;

    return targetFile;
}
