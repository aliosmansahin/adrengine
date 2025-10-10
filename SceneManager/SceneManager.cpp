#include "pch.h"
#include "SceneManager.h"
#include "AssetSaver.h"
#include "InputManager.h"

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
std::shared_ptr<IScene> SceneManager::CreateScene(
    Utils::SceneType sceneType)
{
    //If there is a old scene, release it
    if (openedScene.get()) {
        CloseScene(openedScene->GetSceneId());
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
	std::shared_ptr<IScene> scene = ServiceLocator::Get<IEngine>()->CreateScene();
    if (!scene->CreateScene(sceneId, sceneType)) {
        Logger::Log("E", "Scene could not created");
        return nullptr;
    }

    //Insert it to all scenes
    scenes.insert(std::pair<std::string, std::string>(scene->GetSceneId(), scene->GetSceneName()));

    //Save the scene
    AssetSaver::SaveSceneToFile(scene->ToJson(), ServiceLocator::Get<IProject>()->GetProjectDir(), sceneId);

    openedScene = scene;

    return scene;
}

/*
PURPOSE: Loads the scene and opens it
*/
std::shared_ptr<IScene> SceneManager::LoadScene(std::string sceneId)
{
    //If there is a old scene, release it
    if (openedScene.get()) {
        CloseScene(openedScene->GetSceneId());
    }

    //Load the scene
    nlohmann::json sceneJson = AssetSaver::LoadSceneFromFile(ServiceLocator::Get<IProject>()->GetProjectDir(), sceneId);

    if (sceneJson.is_null())
        return nullptr;

    //Create a scene from its json
    std::shared_ptr<IScene> scene = ServiceLocator::Get<IEngine>()->CreateScene();
    scene->FromJson(sceneJson);

    openedScene = scene;

    return scene;
}

/*
PURPOSE: Saves and closes the scene
*/
bool SceneManager::CloseScene(std::string sceneId)
{
    //If the scene or tab doesn't exists, interrupt the function
    if (!openedScene.get())
        return false;

    //Save the scene
    AssetSaver::SaveSceneToFile(openedScene->ToJson(), ServiceLocator::Get<IProject>()->GetProjectDir(), sceneId);

    //Release the scene
    openedScene->ReleaseScene();
    openedScene = nullptr;

    return true;
}

/*
PURPOSE: Deletes scene object and its folder
*/
bool SceneManager::DeleteScene(std::string sceneId)
{
    //If the scene doesn't exist, interrupt the function
    auto sceneIter = scenes.find(sceneId);
    if (sceneIter == scenes.end())
        return false;

    std::string scenesDir = ServiceLocator::Get<IProject>()->GetProjectDir() + "scenes/";
    std::string sceneDir = scenesDir + sceneId + "/";

    nlohmann::json sceneJson = AssetSaver::LoadSceneFromFile(ServiceLocator::Get<IProject>()->GetProjectDir(), sceneId);

    //Delete each entity that belong to the scene
    if (!sceneJson.is_null()) {
        if (sceneJson.contains("entities")) {
            auto& entities = sceneJson["entities"];

            for (auto& entity : entities) {
                std::string entitiesDir = ServiceLocator::Get<IProject>()->GetProjectDir() + "entities/";
                std::string entityDir = entitiesDir + entity.get<std::string>() + "/";

                //Load entity json
                nlohmann::json entityJson = AssetSaver::LoadEntityFromFile(ServiceLocator::Get<IProject>()->GetProjectDir(), entity.get<std::string>());
                
                //Delete script that belongs to the entity
                std::string scriptId = entityJson.value("scriptId", "");

                if (!scriptId.empty()) {
                    std::string scriptsDir = ServiceLocator::Get<IProject>()->GetProjectDir() + "scripts/";
                    std::string scriptDir = scriptsDir + scriptId + "/";

                    //Delete opened script
                    auto script = ServiceLocator::Get<IVisualScriptManager>()->GetOpenedScripts().find(scriptId);
                    if (script != ServiceLocator::Get<IVisualScriptManager>()->GetOpenedScripts().end()) {
                        ServiceLocator::Get<IVisualScriptManager>()->GetOpenedScripts().erase(script);
                    }

                    std::filesystem::remove_all(scriptDir);
                }

                //Delete entity directory
                std::filesystem::remove_all(entityDir);
            }
        }
    }

    //Check if the scene that will be deleted is an opened scene
    if (openedScene.get() && openedScene->GetSceneId() == sceneId) {
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
PURPOSE: Returns scenes
*/
SCENEMANAGER_API std::map<std::string, std::string>& SceneManager::GetScenes()
{
    return scenes;
}

/*
PURPOSE: Returns opened scene as a smart pointer
*/
SCENEMANAGER_API std::shared_ptr<IScene> SceneManager::GetOpenedScene()
{
    return openedScene;
}