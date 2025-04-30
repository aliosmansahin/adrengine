#include "Engine.h"

bool Engine::InitEngine(GLFWwindow* window)
{
    Logger::Log("P", "Starting engine");
    //set class variables
    this->window = window;

    //graphics engine
    if (!Graphics::GetInstance().InitGraphics(window))
        return false;

    Graphics::GetInstance().CreateFramebuffer(800, 600);

    entityTypes["Entity"] = {
        std::make_shared<Entity>(),
        std::make_shared<EntityParams>()
    };
    entityTypes["Sprite2D"] = {
        std::make_shared<Sprite2D>(),
        std::make_shared<Sprite2DParams>()
    };

    //interface manager
    if (!InterfaceManager::GetInstance().InitInterface(window))
        return false;

    //imput manager
    if (!InputManager::GetInstance().InitEngine(window))
        return false;

    //scene manager
    if (!SceneManager::GetInstance().InitializeManager())
        return false;

    //visual script manager
    if (!VisualScriptManager::GetInstance().InitManager())
        return false;

    //gets screen size
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screenWidth = mode->width;
    screenHeight = mode->height;

    Graphics::GetInstance().LoadTexture("texture", "icon.png");

    //loading existing project
    LoadProject();

    return true;
}

void Engine::Update()
{
    InputManager::GetInstance().Update();

    if(SceneManager::GetInstance().currentScene)
        SceneManager::GetInstance().currentScene->UpdateScene();

    if(WindowAllScenes::GetInstance().pendingDelete) {
        std::string oldSceneId = "";
        if (SceneManager::GetInstance().currentScene)
            oldSceneId = SceneManager::GetInstance().currentScene->sceneId;
        if (SceneManager::GetInstance().currentScene) {
            if (oldSceneId == InterfaceManager::GetInstance().selectedTabId) {
                SceneManager::GetInstance().currentScene = nullptr;
            }
        }
        SceneManager::GetInstance().DeleteScene(WindowAllScenes::GetInstance().selectedSceneId);
        WindowAllScenes::GetInstance().pendingDelete = false;
    }
    if(InterfaceManager::GetInstance().pendingTabDelete) {
        auto tabIter = InterfaceManager::GetInstance().tabs.find(InterfaceManager::GetInstance().deleteTabId);
        if (tabIter != InterfaceManager::GetInstance().tabs.end()) {
            auto tab = tabIter->second.get();
            if (tab->tabType == SceneEditor) {
                std::string openedSceneId = tab->id;
                std::string oldSceneId = "";
                if (SceneManager::GetInstance().currentScene)
                    oldSceneId = SceneManager::GetInstance().currentScene->sceneId;
                if (SceneManager::GetInstance().currentScene && oldSceneId == openedSceneId) {
                    SceneManager::GetInstance().currentScene = nullptr;
                    InterfaceManager::GetInstance().openedTab = nullptr;
                }
                SceneManager::GetInstance().CloseScene(openedSceneId);
            }
            else if (tab->tabType == VisualScriptEditor) {
                std::string openedScriptId = tab->id;
                std::string oldScriptId = "";
                if (VisualScriptManager::GetInstance().currentScript)
                    oldScriptId = VisualScriptManager::GetInstance().currentScript->scriptId;
                if (VisualScriptManager::GetInstance().currentScript && oldScriptId == openedScriptId) {
                    VisualScriptManager::GetInstance().currentScript = nullptr;
                    InterfaceManager::GetInstance().openedTab = nullptr;
                }
                VisualScriptManager::GetInstance().CloseScript(openedScriptId);
            }
        }
        InterfaceManager::GetInstance().pendingTabDelete = false;
    }
}

void Engine::Draw()
{
    /* Render here */
    Graphics::GetInstance().Clear();

    InterfaceManager::GetInstance().StartFrame();
    InterfaceManager::GetInstance().DrawInterface();
    InterfaceManager::GetInstance().EndFrame();
    InterfaceManager::GetInstance().UpdateViewportContext();
}

void Engine::CloseEngine()
{
    //SaveProject();


    InputManager::GetInstance().ReleaseEngine();
    InterfaceManager::GetInstance().CloseInterface();
    Graphics::GetInstance().ReleaseGraphics();
    SceneManager::GetInstance().ClearManager();
    VisualScriptManager::GetInstance().ReleaseManager();
    entityTypes.clear();
    Logger::Log("P", "Cleared engine");
}

void Engine::LoadProject()
{
    std::string projectDir = projectPath + projectName + "/";
    std::string projectFile = projectDir + projectName + ".adrengineproject";
    std::ifstream file(projectFile);

    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading project asset.");
        return;
    }
    
    nlohmann::json projectJson;
    file >> projectJson;
    file.close();

    //TODO: load texture ids


    for (auto& scene : projectJson["scenes"]) {
        SceneManager::GetInstance().scenes.insert(std::pair<std::string, std::string>(scene, scene));
    }
    for (auto& script : projectJson["scripts"]) {
        VisualScriptManager::GetInstance().LoadScript(script);
    }
    for (auto& tab : projectJson["opened-tabs"]) {
        //TODO: tablarý ve sahneleri sync etmek, selectedTab daki deðiþiklikleri kontrol et, deðiþiklik varsa currentScene ve ya currentScript deðiþkenlerini kontrol et.
        //TODO: loading nodes
        if (!tab["type"].empty()) {
            std::string tabId = tab["id"];
            std::string tabType = tab["type"];
            std::string currentTabId = projectJson.value("current-tab", "");
            if (tabType == "SceneEditor") {
                Scene* scene = SceneManager::GetInstance().LoadScene(tabId);
                if (!currentTabId.empty()) {
                    if (tabId == currentTabId) {
                        auto iter = InterfaceManager::GetInstance().tabs.find(tabId);
                        if (iter != InterfaceManager::GetInstance().tabs.end()) {
                            SceneManager::GetInstance().currentScene = scene;
                            InterfaceManager::GetInstance().openedTab = iter->second.get();
                            InterfaceManager::GetInstance().selectedTabId = iter->second->id;
                        }
                    }
                }
            }
            else if(tabType == "VisualScriptEditor") {
                auto result = VisualScriptManager::GetInstance().OpenScript(tabId);
                std::cout << tabId << std::endl;
                if (!currentTabId.empty()) {
                    if (tabId == currentTabId) {
                        auto iter = InterfaceManager::GetInstance().tabs.find(tabId);
                        if (iter != InterfaceManager::GetInstance().tabs.end()) {
                            VisualScriptManager::GetInstance().currentScript = result.first;
                            InterfaceManager::GetInstance().openedTab = iter->second.get();
                            InterfaceManager::GetInstance().selectedTabId = iter->second->id;
                        }
                    }
                }
            }
        }
    }
    //std::cout << VisualScriptManager::GetInstance().openedScripts.find("script0")->second->scriptId << std::endl;

}

void Engine::SaveProject()
{ 
    std::string projectDir = projectPath + projectName + "/";
    std::string projectFile = projectDir + projectName + ".adrengineproject";

    AssetSaver::SaveProjectToFile(projectFile);

    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string entitiesDir = projectDir + "entities/";
    std::filesystem::create_directory(entitiesDir);
    std::string scriptsDir = projectDir + "scripts/";
    std::filesystem::create_directory(scriptsDir);
    for (auto& sceneIter : SceneManager::GetInstance().openedScenes) {
        auto scene = sceneIter.second.get();
        std::string sceneDir = scenesDir + scene->sceneId + "/";
        std::filesystem::create_directory(sceneDir);
        std::string sceneFile = sceneDir + scene->sceneId + ".adrenginescene";
        AssetSaver::SaveSceneToFile(scene, sceneFile);

        if (scene->GetEntityManager()) {
            for (auto& entity : scene->GetEntityManager()->GetEntities()) {
                std::string entityDir = entitiesDir + entity.second->GetEntityParams()->id + "/";
                std::filesystem::create_directory(entityDir);
                std::string entityFile = entityDir + entity.second->GetEntityParams()->id + ".adrengineentity";

                AssetSaver::SaveEntityToFile(entity.second.get(), entityFile);
            }
        }
    }

    for (auto& scriptIter : VisualScriptManager::GetInstance().openedScripts) {
        auto script = scriptIter.second.get();
        std::string scriptDir = scriptsDir + script->scriptId + "/";
        std::filesystem::create_directory(scriptDir);
        std::string scriptFile = scriptDir + script->scriptId + ".adrenginescript";
        AssetSaver::SaveScriptToFile(script, scriptFile);
    }
}

void Engine::SyncTabAndScene(Scene* scene, Tab* tab, std::string currentSceneId)
{
    //tab->id = scene->sceneId;
    //tab->tabType = SceneEditor;
    //if (scene->sceneId == currentSceneId) {
    //    InterfaceManager::GetInstance().openedTab = tab;
    //    SceneManager::GetInstance().currentScene = scene;
    //}

    //auto& openedScenes = SceneManager::GetInstance().openedScenes;
    //auto openedScene = std::find_if(openedScenes.begin(), openedScenes.end(), [&scene](const std::unique_ptr<Scene>& s) {
    //    return s->sceneId == scene->sceneId;
    //    });
    //if (openedScene == openedScenes.end()) {
    //    /*SceneManager::GetInstance().openedScenes.push_back(std::unique_ptr<Scene>(scene));

    //    InterfaceManager::GetInstance().tabs.push_back(std::unique_ptr<Tab>(tab));*/
    //}
}

Engine& Engine::GetInstance()
{
    static Engine engine;
    return engine;
}
