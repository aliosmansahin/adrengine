#include "SceneManager.h"

bool SceneManager::InitializeManager()
{
    //CreateScene();
    return true;
}

void SceneManager::ClearManager()
{
    openedScenes.clear();
    scenes.clear();
    Logger::Log("P", "Cleared scene manager");
}

bool SceneManager::CreateScene()
{
    int index = 0;
    std::string sceneId;
    while (true) {
        sceneId = "scene" + std::to_string(index);
        if (scenes.find(sceneId) == scenes.end())
            break;
        ++index;
    }

    Scene* scene = new Scene();
    if (!scene->CreateScene(sceneId)) {
        Logger::Log("E", "Scene could not created");
        return false;
    }

    scenes.insert(std::pair<std::string, std::string>(scene->sceneId, scene->sceneName));

    std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string sceneDir = scenesDir + scene->sceneId + "/";
    std::filesystem::create_directory(sceneDir);
    std::string sceneFile = sceneDir + scene->sceneId + ".adrenginescene";

    AssetSaver::SaveSceneToFile(scene, sceneFile);

    openedScenes.insert(std::pair<std::string, std::unique_ptr<Scene>>(scene->sceneId, std::unique_ptr<Scene>(scene)));

    Tab* tab = new Tab();
    tab->id = sceneId;
    tab->tabType = SceneEditor;

    InterfaceManager::GetInstance().tabs.insert(std::pair<std::string, std::unique_ptr<Tab>>(tab->id, std::unique_ptr<Tab>(tab)));

    currentScene = scene;
    InterfaceManager::GetInstance().openedTab = tab;
    InterfaceManager::GetInstance().selectedTabId = tab->id;

    std::string projectFile = projectDir + "project.adrengineproject";

    AssetSaver::SaveProjectToFile(projectFile);

    return true;
}

Scene* SceneManager::LoadScene(std::string sceneId)
{
    auto sceneIter = openedScenes.find(sceneId);
    if (sceneIter != openedScenes.end()) {
        return sceneIter->second.get();
    }
    auto tabIter = InterfaceManager::GetInstance().tabs.find(sceneId);
    if (tabIter != InterfaceManager::GetInstance().tabs.end()) {
        return nullptr;
    }
    std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string sceneDir = scenesDir + sceneId + "/";
    std::filesystem::create_directory(sceneDir);
    std::string sceneFile = sceneDir + sceneId + ".adrenginescene";

    Scene* scene = AssetSaver::LoadSceneFromFile(sceneFile);
    if (!scene)
        return nullptr;

    openedScenes.insert(std::pair<std::string, std::unique_ptr<Scene>>(scene->sceneId, std::unique_ptr<Scene>(scene)));

    Tab* tab = new Tab();
    tab->id = sceneId;
    tab->tabType = SceneEditor;

    InterfaceManager::GetInstance().tabs.insert(std::pair<std::string, std::unique_ptr<Tab>>(tab->id, std::unique_ptr<Tab>(tab)));

    return scene;
}

bool SceneManager::CloseScene(std::string sceneId)
{
    auto sceneIter = openedScenes.find(sceneId);
    if (sceneIter == openedScenes.end()) {
        return false;
    }
    auto tabIter = InterfaceManager::GetInstance().tabs.find(sceneId);
    if (tabIter == InterfaceManager::GetInstance().tabs.end()) {
        return false;
    }
    auto scene = sceneIter->second.get();
        
    std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string sceneDir = scenesDir + sceneId + "/";
    std::filesystem::create_directory(sceneDir);
    std::string sceneFile = sceneDir + sceneId + ".adrenginescene";
    AssetSaver::SaveSceneToFile(scene, sceneFile);

    scene->ReleaseScene();
    openedScenes.erase(sceneIter);

    InterfaceManager::GetInstance().tabs.erase(tabIter);

    std::string projectFile = projectDir + "project.adrengineproject";

    AssetSaver::SaveProjectToFile(projectFile);
    return true;
}

bool SceneManager::DeleteScene(std::string sceneId)
{
    auto sceneIter = scenes.find(sceneId);
    if (sceneIter == scenes.end()) {
        return false;
    }
    auto openedIter = openedScenes.find(sceneId);
    if (openedIter != openedScenes.end()) {
        openedIter->second->ReleaseScene();
        openedScenes.erase(openedIter);
    }
    auto tabIter = InterfaceManager::GetInstance().tabs.find(sceneId);
    if (tabIter != InterfaceManager::GetInstance().tabs.end()) {
        auto& tab = tabIter->second;
        std::string openedSceneId = tab->id;
        std::string oldSceneId = "";
        if (SceneManager::GetInstance().currentScene)
            oldSceneId = SceneManager::GetInstance().currentScene->sceneId;
        if (SceneManager::GetInstance().currentScene && oldSceneId == openedSceneId) {
            SceneManager::GetInstance().currentScene = nullptr;
        }
        InterfaceManager::GetInstance().tabs.erase(tabIter);
    }

    std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scenesDir = projectDir + "scenes/";
    std::string sceneDir = scenesDir + sceneId + "/";
    std::filesystem::remove_all(sceneDir);

    scenes.erase(sceneIter);

    std::string projectFile = projectDir + "project.adrengineproject";

    AssetSaver::SaveProjectToFile(projectFile);

    return true;
}

SceneManager& SceneManager::GetInstance()
{
    static SceneManager manager;
    return manager;
}
