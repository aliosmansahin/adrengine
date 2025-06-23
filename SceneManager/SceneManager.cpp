#include "pch.h"
#include "SceneManager.h"
#include "AssetSaver.h"
#include "InputManager.h"
#include "VisualScriptManager.h"
#include <GLFW/glfw3.h>

/*
PURPOSE: Initializes the scene manager
*/
bool SceneManager::InitializeManager()
{
    return true;
}

/*
PURPOSE: Releases the scene manager
*/
void SceneManager::ClearManager()
{
    //Release opened scene
    if(openedScene)
        openedScene->ReleaseScene();
    openedScene = nullptr;

    //Clear the scene maps
    scenes.clear();

    //Logger
    Logger::Log("P", "Cleared scene manager");
}

/*
PURPOSE: Creates a scene and opens it
*/
bool SceneManager::CreateScene(
    Utils::SceneType sceneType,
    std::string& projectDir)
{
    //If there is a old scene, release it
    if (openedScene.get()) {
        CloseScene(openedScene->sceneId, projectDir);
    }

    //Set the scene id
    int index = 0;
    std::string sceneId;
    while (true) {
        sceneId = "scene" + std::to_string(index);
        if (scenes.find(sceneId) == scenes.end())
            break;
        ++index;
    }

    //Create a scene
    Scene* scene = new Scene();
    if (!scene->CreateScene(sceneId, sceneType)) {
        Logger::Log("E", "Scene could not created");
        return false;
    }

    //Insert it to all scenes
    scenes.insert(std::pair<std::string, std::string>(scene->sceneId, scene->sceneName));

    //Save the scene
    AssetSaver::SaveSceneToFile(scene->ToJson(), projectDir, sceneId);

    openedScene = std::shared_ptr<Scene>(scene);

    return true;
}

/*
PURPOSE: Loads the scene and opens it
*/
Scene* SceneManager::LoadScene(std::string sceneId, std::string& projectDir,
    std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes)
{
    //If there is a old scene, release it
    if (openedScene.get()) {
        CloseScene(openedScene->sceneId, projectDir);
    }

    //Load the scene
    nlohmann::json sceneJson = AssetSaver::LoadSceneFromFile(projectDir, sceneId);

    if (sceneJson.is_null())
        return nullptr;

    //Create a scene from its json
    Scene* scene = new Scene();
    scene->FromJson(sceneJson, projectDir, entityTypes);

    openedScene = std::shared_ptr<Scene>(scene);

    return scene;
}

/*
PURPOSE: Saves and closes the scene
*/
bool SceneManager::CloseScene(std::string sceneId, std::string& projectDir)
{
    //If the scene or tab doesn't exists, interrupt the function
    if (!openedScene.get())
        return false;

    //Save the scene
    AssetSaver::SaveSceneToFile(openedScene->ToJson(), projectDir, sceneId);

    //Release the scene
    openedScene->ReleaseScene();
    openedScene = nullptr;

    return true;
}

/*
PURPOSE: Deletes scene object and its folder
*/
bool SceneManager::DeleteScene(std::string sceneId, std::string& projectDir)
{
    //If the scene doesn't exist, interrupt the function
    auto sceneIter = scenes.find(sceneId);
    if (sceneIter == scenes.end())
        return false;

    std::string scenesDir = projectDir + "scenes/";
    std::string sceneDir = scenesDir + sceneId + "/";

    nlohmann::json sceneJson = AssetSaver::LoadSceneFromFile(projectDir, sceneId);

    //Delete each entity that belong to the scene
    if (!sceneJson.is_null()) {
        if (sceneJson.contains("entities")) {
            auto& entities = sceneJson["entities"];

            for (auto& entity : entities) {
                std::string entitiesDir = projectDir + "entities/";
                std::string entityDir = entitiesDir + entity.get<std::string>() + "/";

                //Load entity json
                nlohmann::json entityJson = AssetSaver::LoadEntityFromFile(projectDir, entity.get<std::string>());
                
                //Delete script that belongs to the entity
                std::string scriptId = entityJson.value("scriptId", "");

                if (!scriptId.empty()) {
                    std::string scriptsDir = projectDir + "scripts/";
                    std::string scriptDir = scriptsDir + scriptId + "/";

                    //Delete opened script
                    auto script = VisualScriptManager::GetInstance().openedScripts.find(scriptId);
                    if (script != VisualScriptManager::GetInstance().openedScripts.end()) {
                        VisualScriptManager::GetInstance().openedScripts.erase(script);
                    }

                    std::filesystem::remove_all(scriptDir);
                }

                //Delete entity directory
                std::filesystem::remove_all(entityDir);
            }
        }
    }

    //Check if the scene that will be deleted is an opened scene
    if (openedScene.get() && openedScene->sceneId == sceneId) {
        //Remove the opened scene
        openedScene->ReleaseScene();
        openedScene = nullptr;
    }

    //Delete scene directory
    std::filesystem::remove_all(sceneDir);

    //Remove scene from scenes map
    scenes.erase(sceneIter);

    return true;
}

/*
PURPOSE: Gets the instance of the class
*/
SceneManager& SceneManager::GetInstance()
{
    static SceneManager manager;
    return manager;
}