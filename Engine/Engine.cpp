#include "pch.h"
#include "Engine.h"

/*
PURPOSE: Initialize engine
*/
bool Engine::InitEngine(GLFWwindow* window)
{
    //log stuff
    Logger::Log("P", "Starting engine");

    //set class variables
    this->window = window;

    //initialize graphics engine
    if (!Graphics::GetInstance().InitGraphics(window))
        return false;

    //create a frame buffer to write it to imgui as a texture
    Graphics::GetInstance().CreateFramebuffer(800, 600);

    //initialize all of entity types into the unordered_map
    InitEntityTypes();
    
    //------ INITIALIZE OTHER ENGINES ------
    
    //interface manager
    if (!InterfaceManager::GetInstance().InitInterface(window))
        return false;

    //input manager
    if (!InputManager::GetInstance().InitEngine())
        return false;

    //scene manager
    if (!SceneManager::GetInstance().InitializeManager())
        return false;

    //visual script manager
    if (!VisualScriptManager::GetInstance().InitManager())
        return false;

    /*
    * Request to get video mode to get width and height
    * 
    * --glfw functions are failing so i am using a request to window to get it from there
    */
    RequestGetVideoMode();

    //load existing project
    LoadProject();

    return true;
}

/*
PURPOSE: Initialize entity types
*/
ENGINE_API void Engine::InitEntityTypes()
{
    entityTypes["Entity"] = {
        std::make_shared<Entity>(),
        std::make_shared<EntityParams>()
    };
    entityTypes["Sprite2D"] = {
        std::make_shared<Sprite2D>(),
        std::make_shared<Sprite2DParams>()
    };
    entityTypes["Object"] = {
        std::make_shared<Object>(),
        std::make_shared<ObjectParams>()
    };
    entityTypes["DirectionalLight"] = {
        std::make_shared<DirectionalLight>(),
        std::make_shared<DirectionalLightParams>()
    };
    entityTypes["SpotLight"] = {
        std::make_shared<SpotLight>(),
        std::make_shared<SpotLightParams>()
    };
    entityTypes["PointLight"] = {
        std::make_shared<PointLight>(),
        std::make_shared<PointLightParams>()
    };
}

/*
PURPOSE: Update engines and other stuff
*/
void Engine::Update()
{
    //update timer to calc delta time
    Timer::Update();

    //calculate ms and fps
    CalcFPSandMS();

    //update scenes
    UpdateCurrentScene();

    //perform tab and scene delete actions
    PerformDeleteActions();
}

/*
PURPOSE: To draw main frame
*/
void Engine::Draw()
{
    Graphics::GetInstance().Clear();
    
    InterfaceManager::GetInstance().StartFrame();

    std::string projectDir = projectPath + projectName + "/";
    
    InterfaceManager::GetInstance().DrawInterface(projectDir, [this]() { SaveProject(); }, entityTypes, FPS, ms);
    
    InterfaceManager::GetInstance().EndFrame();

    InterfaceManager::GetInstance().UpdateViewportContext();
}

/*
PURPOSE: Close all engines include this one
*/
void Engine::CloseEngine()
{
    //SaveProject();

    InputManager::GetInstance().ReleaseEngine();
    InterfaceManager::GetInstance().CloseInterface();
    SceneManager::GetInstance().ClearManager();
    VisualScriptManager::GetInstance().ReleaseManager();
    entityTypes.clear();
    Graphics::GetInstance().ReleaseGraphics();
    Logger::Log("P", "Cleared engine");
}

/*
PURPOSE: To load existing project
*/
void Engine::LoadProject()
{
    //set up the project file and load it
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

    //loads asset database
    AssetDatabase::GetInstance().LoadDatabase(projectDir + "asset_database.adrenginedatabase");

    //loads all scenes and scripts that belong to the project
    for (auto& scene : projectJson["scenes"]) {
        SceneManager::GetInstance().scenes.insert(std::pair<std::string, std::string>(scene, scene));
    }
    for (auto& script : projectJson["scripts"]) {
        VisualScriptManager::GetInstance().LoadScript(script, projectDir, InterfaceManager::GetInstance().tabs);
    }

    //loads all opened tabs and load scene or script depends on the tabType
    for (auto& tab : projectJson["opened-tabs"]) {
        //TODO: i might sync the tabs to the scene or script(which one is opened). However it won't be here i will write it in interfacemanager
        if (!tab["type"].empty()) {
            std::string tabId = tab["id"];
            std::string tabType = tab["type"];
            std::string currentTabId = projectJson.value("current-tab", "");
            Scene* scene = nullptr;
            std::shared_ptr<VisualScript> script = nullptr;
            if (tabType == "SceneEditor") {
                //it is a scene so load the scene
                scene = SceneManager::GetInstance().LoadScene(tabId, projectDir, InterfaceManager::GetInstance().tabs, entityTypes);
            }
            else if (tabType == "VisualScriptEditor") {
                //it is a script so load the script
                script = VisualScriptManager::GetInstance().OpenScript(tabId, InterfaceManager::GetInstance().tabs).first;
            }
            //check if the loaded tab is the last opened one
            if (!currentTabId.empty()) {
                if (tabId == currentTabId) {
                    //set the current scene or script
                    if (scene)
                        SceneManager::GetInstance().currentScene = scene;
                    if (script)
                        VisualScriptManager::GetInstance().currentScript = script;

                    //set the current tab
                    auto iter = InterfaceManager::GetInstance().tabs.find(tabId);
                    if (iter != InterfaceManager::GetInstance().tabs.end()) {
                        InterfaceManager::GetInstance().openedTab = iter->second.get();
                        InterfaceManager::GetInstance().selectedTabId = iter->second->id;
                    }
                }
            }
        }
    }
}

/*
PURPOSE: To save the project
*/
void Engine::SaveProject()
{
    //saves the project to the project file
    std::string projectDir = projectPath + projectName + "/";
    std::string projectFile = projectDir + projectName + ".adrengineproject";

    auto openedTab = InterfaceManager::GetInstance().openedTab;

    std::string openedTabId = "";
    if (openedTab)
        openedTabId = openedTab->id;
    
    nlohmann::json projectJson = CreateProjectJson(openedTabId, SceneManager::GetInstance().scenes, VisualScriptManager::GetInstance().scripts, InterfaceManager::GetInstance().tabs);

    AssetSaver::SaveProjectToFile(projectFile, projectJson);

    //save assets
    AssetDatabase::GetInstance().SaveDatabase(projectDir + "asset_database.adrenginedatabase");

    //saves each opened-scenes and each entity that belong to the scene
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
        AssetSaver::SaveSceneToFile(scene->ToJson(), sceneFile, projectDir);

        if (scene->GetEntityManager()) {
            for (auto& entity : scene->GetEntityManager()->GetEntities()) {
                std::string entityDir = entitiesDir + entity.second->GetEntityParams()->id + "/";
                std::filesystem::create_directory(entityDir);
                std::string entityFile = entityDir + entity.second->GetEntityParams()->id + ".adrengineentity";

                AssetSaver::SaveEntityToFile(entity.second->ToJson(), entityFile);
            }
        }
    }

    //saves each opened-scripts
    for (auto& scriptIter : VisualScriptManager::GetInstance().openedScripts) {
        auto script = scriptIter.second.get();
        std::string scriptDir = scriptsDir + script->scriptId + "/";
        std::filesystem::create_directory(scriptDir);
        std::string scriptFile = scriptDir + script->scriptId + ".adrenginescript";
        AssetSaver::SaveScriptToFile(script->ToJson(), scriptFile);
    }
}

/*
PURPOSE: To calculate FPS and MS
*/
ENGINE_API void Engine::CalcFPSandMS()
{
    static double prevTime = Timer::GetCurTime();
    double currentTime = Timer::GetCurTime();
    float timeDiff = (float)currentTime - (float)prevTime;

    static int counter = 0;
    counter++;

    if (timeDiff >= 1.0f / 30.0f) {
        FPS = 1.0f / timeDiff * counter;
        ms = timeDiff / counter * 1000;
        prevTime = currentTime;
        counter = 0;
    }
}

/*
PURPOSE: To update current scene
*/
ENGINE_API void Engine::UpdateCurrentScene()
{
    if (SceneManager::GetInstance().currentScene) {
        std::string projectDir = projectPath + projectName + "/"; //TODO: control from one point
        SceneManager::GetInstance().currentScene->UpdateScene(
            WindowGameViewport::GetInstance().isPlaying,
            WindowGameViewport::GetInstance().isHovered,
            screenWidth,
            screenHeight,
            (int)WindowGameViewport::GetInstance().window_width,
            (int)WindowGameViewport::GetInstance().window_height,
            WindowScene::GetInstance().focused,
            WindowScene::GetInstance().deletePressed,
            WindowScene::GetInstance().pendingDelete,
            WindowScene::GetInstance().selectedId,
            []() {
                WindowEntityProperties::GetInstance().SelectEntity(nullptr);
            },
            projectDir);
    }
}

/*
PURPOSE: To perform delete actions like deleting scene
*/
ENGINE_API void Engine::PerformDeleteActions()
{
    //store project directory
    std::string projectDir = projectPath + projectName + "/";

    //perform deleting scene
    if (WindowAllScenes::GetInstance().pendingDelete) {
        std::string oldSceneId = "";

        if (SceneManager::GetInstance().currentScene) {
            oldSceneId = SceneManager::GetInstance().currentScene->sceneId;
            if (oldSceneId == InterfaceManager::GetInstance().selectedTabId) {
                SceneManager::GetInstance().currentScene = nullptr;
            }
        }
        SceneManager::GetInstance().DeleteScene(WindowAllScenes::GetInstance().selectedSceneId, projectDir, InterfaceManager::GetInstance().tabs, InterfaceManager::GetInstance().openedTab, VisualScriptManager::GetInstance().scripts);
        WindowAllScenes::GetInstance().pendingDelete = false;
    }

    //perform deleting tab
    if (InterfaceManager::GetInstance().pendingTabDelete) {
        auto tabIter = InterfaceManager::GetInstance().tabs.find(InterfaceManager::GetInstance().deleteTabId);
        if (tabIter != InterfaceManager::GetInstance().tabs.end()) {
            auto tab = tabIter->second.get();
            //if tabType is scene
            if (tab->tabType == SceneEditor) {
                std::string openedSceneId = tab->id;
                std::string oldSceneId = "";
                if (SceneManager::GetInstance().currentScene)
                    oldSceneId = SceneManager::GetInstance().currentScene->sceneId;
                if (SceneManager::GetInstance().currentScene && oldSceneId == openedSceneId) {
                    SceneManager::GetInstance().currentScene = nullptr;
                    InterfaceManager::GetInstance().openedTab = nullptr;
                }
                SceneManager::GetInstance().CloseScene(openedSceneId, projectDir, InterfaceManager::GetInstance().tabs, InterfaceManager::GetInstance().openedTab, VisualScriptManager::GetInstance().scripts);
            }
            //if tabType is visualscript
            else if (tab->tabType == VisualScriptEditor) {
                std::string openedScriptId = tab->id;
                std::string oldScriptId = "";
                if (VisualScriptManager::GetInstance().currentScript)
                    oldScriptId = VisualScriptManager::GetInstance().currentScript->scriptId;
                if (VisualScriptManager::GetInstance().currentScript && oldScriptId == openedScriptId) {
                    VisualScriptManager::GetInstance().currentScript = nullptr;
                    InterfaceManager::GetInstance().openedTab = nullptr;
                }
                VisualScriptManager::GetInstance().CloseScript(openedScriptId, projectDir, InterfaceManager::GetInstance().tabs, InterfaceManager::GetInstance().openedTab, SceneManager::GetInstance().scenes);
            }
        }
        InterfaceManager::GetInstance().pendingTabDelete = false;
    }
}

/*
PURPOSE: To request to get screen resolution
*/
ENGINE_API void Engine::RequestGetVideoMode()
{
    requestGetVideoMode = true;
}

/*
PURPOSE: To get answer the window class and set screen resolution
*/
ENGINE_API void Engine::AnswerGetVideoMode(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}

/*
PURPOSE: To get if the request of getting resolution is requested
*/
ENGINE_API bool Engine::GetRequestGetVideoMode()
{
    //once this was requested even request is true, change it to false but return old value
    bool tmp = requestGetVideoMode;
    requestGetVideoMode = false;
    return tmp;
}

/*
PURPOSE: To get the instance of the engine
*/
Engine& Engine::GetInstance()
{
    static Engine engine;
    return engine;
}
