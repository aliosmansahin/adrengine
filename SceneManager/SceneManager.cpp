#include "pch.h"
#include "SceneManager.h"
#include "AssetSaver.h"
#include "InputManager.h"
#include <GLFW/glfw3.h>

/*
PURPOSE: Creates a project json content
TODO: This function will be moved to another place like "common" project (I haven't decided it yet)
*/
SCENEMANAGER_API nlohmann::json CreateProjectJson(
    std::string openedTabId,
	std::map<std::string, std::string>& scenes,
	std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts,
	std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs) {

    nlohmann::json projectJson;

    //Current tab
    projectJson["current-tab"] = openedTabId;

    //All scenes
    for (auto& sceneIter : scenes) {
        auto& scene = sceneIter.first;
        projectJson["scenes"].push_back(scene);
    }

    //All scripts
    for (auto& scriptIter : scripts) {
        auto& script = scriptIter.first;
        projectJson["scripts"].push_back(script);
    }

    //Opened tabs
    for (auto& tabIter : tabs) {
        auto& tabId = tabIter.first;
        auto& tab = tabIter.second;
        nlohmann::json a;
        a["id"] = tabId;
        if (tab->tabType == SceneEditor)
            a["type"] = "SceneEditor";
        if (tab->tabType == VisualScriptEditor)
            a["type"] = "VisualScriptEditor";
        projectJson["opened-tabs"].push_back(a);
    }

    return projectJson;
}

/*
PURPOSE: Initializes the scene manager
*/
bool SceneManager::InitializeManager()
{
    //CreateScene();
    return true;
}

/*
PURPOSE: Releases the scene manager
*/
void SceneManager::ClearManager()
{
    //Release each scene
    for (auto& scene : openedScenes)
        scene.second->ReleaseScene();

    //currentScene is nullptr anymore
    currentScene = nullptr;

    //Clear the scene maps
    openedScenes.clear();
    scenes.clear();

    //Logger
    Logger::Log("P", "Cleared scene manager");
}

/*
PURPOSE: Creates a scene and opens it
*/
bool SceneManager::CreateScene(
    SceneType sceneType,
    std::string& projectDir,
    std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
    Tab*& openedTab,
    std::string& selectedTabId,
    std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts)
{
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
    //std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string sceneDir = scenesDir + scene->sceneId + "/";
    std::filesystem::create_directory(sceneDir);
    std::string sceneFile = sceneDir + scene->sceneId + ".adrenginescene";

    AssetSaver::SaveSceneToFile(scene->ToJson(), sceneFile, projectDir);

    //Insert it to opened scenes to open it
    openedScenes.insert(std::pair<std::string, std::unique_ptr<Scene>>(scene->sceneId, std::unique_ptr<Scene>(scene)));

    //Create a new tab and insert it to all tabs
    Tab* tab = new Tab();
    tab->id = sceneId;
    tab->tabType = SceneEditor;

    tabs.insert(std::pair<std::string, std::unique_ptr<Tab>>(tab->id, std::unique_ptr<Tab>(tab)));

    //Set the current scene and tab to the new scene and tab
    currentScene = scene;
    openedTab = tab;
    selectedTabId = tab->id;

    //Save the project
    std::string projectFile = projectDir + "project.adrengineproject";

    nlohmann::json projectJson = CreateProjectJson(openedTab->id, scenes, scripts, tabs);

    AssetSaver::SaveProjectToFile(projectFile, projectJson);

    return true;
}

/*
PURPOSE: Loads the scene and opens it
*/
Scene* SceneManager::LoadScene(std::string sceneId, std::string& projectDir,
    std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
    std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes)
{
    //If the scene or tab exists, don't create a new one
    auto sceneIter = openedScenes.find(sceneId);
    if (sceneIter != openedScenes.end()) {
        return sceneIter->second.get();
    }
    auto tabIter = tabs.find(sceneId);
    if (tabIter != tabs.end()) {
        return nullptr;
    }

    //Load the scene
    //std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string sceneDir = scenesDir + sceneId + "/";
    std::filesystem::create_directory(sceneDir);
    std::string sceneFile = sceneDir + sceneId + ".adrenginescene";

    nlohmann::json sceneJson = AssetSaver::LoadSceneFromFile(sceneFile, projectDir);

    if (sceneJson.is_null())
        return nullptr;

    //Create a scene from its json
    Scene* scene = new Scene();
    scene->FromJson(sceneJson, projectDir, entityTypes);

    //Insert the scene to opened scenes to open it
    openedScenes.insert(std::pair<std::string, std::unique_ptr<Scene>>(scene->sceneId, std::unique_ptr<Scene>(scene)));

    //Create a new tab
    Tab* tab = new Tab();
    tab->id = sceneId;
    tab->tabType = SceneEditor;

    //Insert the tab to tabs map
    tabs.insert(std::pair<std::string, std::unique_ptr<Tab>>(tab->id, std::unique_ptr<Tab>(tab)));

    return scene;
}

/*
PURPOSE: Saves and closes the scene
*/
bool SceneManager::CloseScene(std::string sceneId, std::string& projectDir,
    std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
    Tab*& openedTab,
    std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts)
{
    //If the scene or tab doesn't exists, interrupt the function
    auto sceneIter = openedScenes.find(sceneId);
    if (sceneIter == openedScenes.end()) {
        return false;
    }
    auto tabIter = tabs.find(sceneId);
    if (tabIter == tabs.end()) {
        return false;
    }

    auto scene = sceneIter->second.get();
    
    //Save the scene
    //std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string sceneDir = scenesDir + sceneId + "/";
    std::filesystem::create_directory(sceneDir);
    std::string sceneFile = sceneDir + sceneId + ".adrenginescene";
    AssetSaver::SaveSceneToFile(scene->ToJson(), sceneFile, projectDir);

    //Release the scene
    scene->ReleaseScene();

    //Remove the scene from opened scenes
    openedScenes.erase(sceneIter);

    //Remove the tab from tabs
    tabs.erase(tabIter);

    //Save project
    std::string projectFile = projectDir + "project.adrengineproject";

    std::string openedTabId = "";

    if (openedTab) {
        openedTabId = openedTab->id;
    }

    nlohmann::json projectJson = CreateProjectJson(openedTabId, scenes, scripts, tabs);
    AssetSaver::SaveProjectToFile(projectFile, projectJson);
    return true;
}

/*
PURPOSE: Deletes scene object and its folder
*/
bool SceneManager::DeleteScene(std::string sceneId, std::string& projectDir,
    std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
    Tab*& openedTab,
    std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts)
{
    //If the scene doesn't exist, interrupt the function
    auto sceneIter = scenes.find(sceneId);
    if (sceneIter == scenes.end()) {
        return false;
    }

    //Remove the scene from opened scenes
    auto openedIter = openedScenes.find(sceneId);
    if (openedIter != openedScenes.end()) {
        openedIter->second->ReleaseScene();
        openedScenes.erase(openedIter);
    }

    //Remove the tab from tabs
    auto tabIter = tabs.find(sceneId);
    if (tabIter != tabs.end()) {
        auto& tab = tabIter->second;
        std::string openedSceneId = tab->id;
        std::string oldSceneId = "";

        //If closing scene is the opened scene
        if (SceneManager::GetInstance().currentScene)
            oldSceneId = SceneManager::GetInstance().currentScene->sceneId;
        if (SceneManager::GetInstance().currentScene && oldSceneId == openedSceneId) {
            SceneManager::GetInstance().currentScene = nullptr;
        }

        //Remove tab
        tabs.erase(tabIter);
        if (openedTab)
            openedTab = nullptr;
    }
    
    //Delete scene directory
    //std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scenesDir = projectDir + "scenes/";
    std::string sceneDir = scenesDir + sceneId + "/";
    std::filesystem::remove_all(sceneDir);

    //Remove scene from scenes map
    scenes.erase(sceneIter);

    //Save project
    std::string projectFile = projectDir + "project.adrengineproject";
    std::string openedTabId = "";

    if (openedTab) {
        openedTabId = openedTab->id;
    }

    nlohmann::json projectJson = CreateProjectJson(openedTabId, scenes, scripts, tabs);
    AssetSaver::SaveProjectToFile(projectFile, projectJson);

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