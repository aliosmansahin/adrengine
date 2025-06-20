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

    //initialize all of entity types into the unordered_map
    InitEntityTypes();

    //------ INITIALIZE SUB ENGINES ------
    
    //initialize graphics engine
    if (!Graphics::GetInstance().InitGraphics(window))
        return false;

    //interface manager
    ImGuiContext* context = nullptr;
    ImNodesContext* nodesContext = nullptr;
    if (!InterfaceManager::GetInstance().InitInterface(window, context, nodesContext))
        return false;

    //input manager
    if (!InputManager::GetInstance().InitEngine(window, context))
        return false;

    //scene manager
    if (!SceneManager::GetInstance().InitializeManager())
        return false;

    //visual script manager
    if (!VisualScriptManager::GetInstance().InitManager(context, nodesContext))
        return false;

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
    entityTypes["TileMap"] = {
        std::make_shared<TileMap>(),
        std::make_shared<TileMapParams>()
    };
    entityTypes["FlipBook"] = {
        std::make_shared<FlipBook>(),
        std::make_shared<FlipBookParams>()
    };
    entityTypes["Camera"] = {
        std::make_shared<Camera>(),
        std::make_shared<CameraParams>()
    };
}

/*
PURPOSE: Update engines and other stuff
*/
void Engine::Update()
{
    //Update timer to calc delta time
    Timer::Update();

    /*
        Update inputs,
        Keys, mouse buttons and mouse position are updating via Update function
    */
    GLFWwindow* window = InterfaceManager::GetInstance().GetFocusedViewport();
    if (window) {
        InputManager::GetInstance().Update(window);
    }

    //Calculate ms and fps
    CalcFPSandMS();

    //Get screen width
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screenWidth = mode->width;
    screenHeight = mode->height;

    //Update scenes
    UpdateCurrentScene();

    //Perform tab and scene delete actions
    PerformDeleteActions();
}

/*
PURPOSE: To draw main frame
*/
void Engine::Draw()
{
    Graphics::GetInstance().Clear();
    
    InterfaceManager::GetInstance().StartFrame();

    InterfaceManager::GetInstance().DrawInterface(projectDir, [this]() { SaveProject(); }, entityTypes, FPS, ms, screenWidth, screenHeight);
    
    InterfaceManager::GetInstance().EndFrame();

    InterfaceManager::GetInstance().UpdateViewportContext();
}

/*
PURPOSE: Close all engines include this one
*/
void Engine::CloseEngine()
{
    InputManager::GetInstance().ReleaseEngine();
    InterfaceManager::GetInstance().CloseInterface();
    SceneManager::GetInstance().ClearManager();
    VisualScriptManager::GetInstance().ReleaseManager();
    Graphics::GetInstance().ReleaseGraphics();
    entityTypes.clear();
    Logger::Log("P", "Cleared engine");
}

/*
PURPOSE: To load existing project
*/
void Engine::LoadProject()
{
    //set up the project file and load it
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

    if (projectJson.contains("opened-scene")) {
        //it is a scene so load the scene
        Scene* scene = SceneManager::GetInstance().LoadScene(projectJson["opened-scene"], projectDir, entityTypes);
        if (!scene)
            return;

        //Create a tab and insert it to tabs
        std::shared_ptr<Utils::Tab> tab = std::make_shared<Utils::Tab>();
        tab->id = scene->sceneId;
        tab->tabType = Utils::SceneEditor;
        InterfaceManager::GetInstance().tabs.insert(std::pair<std::string, std::shared_ptr<Utils::Tab>>(tab->id, tab));

        InterfaceManager::GetInstance().openedTab = tab.get();
        InterfaceManager::GetInstance().selectedTabId = tab->id;
    }
}

/*
PURPOSE: To save the project
*/
void Engine::SaveProject()
{
    //saves the project to the project file
    std::string projectFile = projectDir + projectName + ".adrengineproject";

    //get scene id
    std::string sceneId = "";
    if (SceneManager::GetInstance().openedScene.get())
        sceneId = SceneManager::GetInstance().openedScene->sceneId;

    nlohmann::json projectJson = Utils::CreateProjectJson(SceneManager::GetInstance().scenes, sceneId);

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

    Scene* scene = SceneManager::GetInstance().openedScene.get();
    if (scene) {
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
    if (SceneManager::GetInstance().openedScene) {
        //We will use tileMapBrush when "start drawing" button clicked
        TileMap* edittingTileMap = WindowTileMapBrush::GetInstance().editing ? WindowTileMapBrush::GetInstance().editingTileMap : nullptr;

        SceneManager::GetInstance().openedScene->UpdateScene(
            WindowGameViewport::GetInstance().isPlaying,
            WindowGameViewport::GetInstance().isHovered,
            WindowGameViewport::GetInstance().isFocused,
            screenWidth,
            screenHeight,
            (int)WindowGameViewport::GetInstance().window_width,
            (int)WindowGameViewport::GetInstance().window_height,
            WindowScene::GetInstance().focused,
            WindowScene::GetInstance().deletePressed,
            WindowScene::GetInstance().pendingDelete,
            WindowScene::GetInstance().selectedId,
            [](std::string scriptId) {
                //Delete tab
                auto iter = InterfaceManager::GetInstance().tabs.find(scriptId);
                if (iter != InterfaceManager::GetInstance().tabs.end()) {
                    InterfaceManager::GetInstance().tabs.erase(iter);
                }

                //Select nothing
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
    //perform deleting scene
    if (WindowAllScenes::GetInstance().pendingDelete) {
        SceneManager::GetInstance().DeleteScene(WindowAllScenes::GetInstance().selectedSceneId, projectDir);

        //Clear all tabs
        InterfaceManager::GetInstance().tabs.clear();
        InterfaceManager::GetInstance().openedTab = nullptr;

        //Save project
        std::string projectFile = projectDir + "project.adrengineproject";

        std::string openedSceneId = "";
        if (SceneManager::GetInstance().openedScene)
            openedSceneId = SceneManager::GetInstance().openedScene->sceneId;

        nlohmann::json projectJson = Utils::CreateProjectJson(SceneManager::GetInstance().scenes, openedSceneId);
        AssetSaver::SaveProjectToFile(projectFile, projectJson);

        WindowAllScenes::GetInstance().pendingDelete = false;
    }

    //perform deleting tab
    if (InterfaceManager::GetInstance().pendingTabDelete) {
        auto tabIter = InterfaceManager::GetInstance().tabs.find(InterfaceManager::GetInstance().deleteTabId);
        if (tabIter != InterfaceManager::GetInstance().tabs.end()) {
            auto tab = tabIter->second.get();
            //if tabType is scene
            if (tab->tabType == Utils::SceneEditor) {
                SceneManager::GetInstance().CloseScene(tab->id, projectDir);

                //Remove the tab from tabs
                InterfaceManager::GetInstance().tabs.clear();
                InterfaceManager::GetInstance().openedTab = nullptr;

                //Save project
                std::string projectFile = projectDir + "project.adrengineproject";

                std::string openedSceneId = "";
                if (SceneManager::GetInstance().openedScene)
                    openedSceneId = SceneManager::GetInstance().openedScene->sceneId;

                nlohmann::json projectJson = Utils::CreateProjectJson(SceneManager::GetInstance().scenes, openedSceneId);
                AssetSaver::SaveProjectToFile(projectFile, projectJson);
            }
            //if tabType is visualscript
            else if (tab->tabType == Utils::VisualScriptEditor) {
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
PURPOSE: To get the instance of the engine
*/
Engine& Engine::GetInstance()
{
    static Engine engine;
    return engine;
}
