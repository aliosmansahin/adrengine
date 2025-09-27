#include "Project.h"
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
    if (!InterfaceManager::GetInstance().InitInterface(window, context, nodesContext))
        return false;

    //Load latest projects
    Project::Get().LoadLatestProjects();

    //TODO: Add Open With option to project files

    return true;
}

/*
PURPOSE: Initialize entity types
    This is for storing entities with a key,
    also new entities will be copied from them, depends on entity type string
*/
void Engine::InitEntityTypes()
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
PURPOSE: Main Update Function of engine
    Calls different functions to handle two different states of opened/not opened project
*/
void Engine::Update()
{
    if (Project::Get().projectOpened) {
        UpdateEngineWhenProjectIsOpened();
    }
    else {
        UpdateEngineWhenProjectIsNotOpened();
    }
}

/*
PURPOSE: Main Draw function for engine
*/
void Engine::Draw()
{
    Graphics::GetInstance().Clear();

    InterfaceManager::GetInstance().StartFrame();

    InterfaceManager::GetInstance().DrawInterface(
        Project::Get().GetProjectDir(),
        Project::Get().GetProjectFileLocation(),
        [this]() {
            Project::Get().SaveProject();
        },
        [this]() {
            Project::Get().CloseProject();
            WindowProjectDialog::GetInstance().ResetInputs();
        }, 
        entityTypes,
        Project::Get().GetLatestProjects(),
        FPS,
        ms,
        screenWidth,
        screenHeight,
        Project::Get().projectOpened
    );

    InterfaceManager::GetInstance().EndFrame();

    InterfaceManager::GetInstance().UpdateViewportContext();
}

/*
PURPOSE: Close all engines include this one
*/
void Engine::CloseEngine()
{
    //Close Project if it is opened
    if(Project::Get().projectOpened)
        Project::Get().CloseProject();

    //Release other engines
    InterfaceManager::GetInstance().CloseInterface();
    Graphics::GetInstance().ReleaseGraphics();

    //Release entity types
    entityTypes.clear();

    //Log
    Logger::Log("P", "Cleared engine");
}

/*
PURPOSE: Calculates FPS and MS
*/
void Engine::CalcFPSandMS()
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
PURPOSE: Updates current scene
*/
void Engine::UpdateCurrentScene()
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

                //Delete opened script
                auto script = VisualScriptManager::GetInstance().openedScripts.find(scriptId);
                if (script != VisualScriptManager::GetInstance().openedScripts.end()) {
                    VisualScriptManager::GetInstance().openedScripts.erase(script);
                }

                //Select nothing
                WindowEntityProperties::GetInstance().SelectEntity(nullptr);
            },
            Project::Get().GetProjectDir());
    }
}

/*
PURPOSE: Performs delete actions like deleting scene
*/
void Engine::PerformDeleteActions()
{
    std::string projectDir = Project::Get().GetProjectDir();
    std::string projectFile = Project::Get().GetProjectFileLocation();

    //perform deleting scene
    PerformSceneDeletion(projectDir, projectFile);

    //perform deleting tab
    PerformTabDeletion(projectDir, projectFile);
}

/*
PURPOSE: Handles updating part of engine if a project is not opened
    This is called by update function of the engine
*/
void Engine::UpdateEngineWhenProjectIsNotOpened()
{
    //Check for creating or opening a project
    HandleProjectOpeningOrCreation();
}

/*
PURPOSE: Handles updating part of engine if a project is opened
    This is called by update function of the engine
*/
void Engine::UpdateEngineWhenProjectIsOpened()
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
PURPOSE: Performs delete actions for scenes
    This function is called by PerformDeleteActions
*/
void Engine::PerformSceneDeletion(std::string& projectDir, std::string& projectFile)
{
    if (WindowAllScenes::GetInstance().pendingDelete) {
        SceneManager::GetInstance().DeleteScene(WindowAllScenes::GetInstance().selectedSceneId, projectDir);

        //Clear all tabs
        InterfaceManager::GetInstance().tabs.clear();
        InterfaceManager::GetInstance().openedTab = nullptr;

        //Save project
        std::string openedSceneId = "";
        if (SceneManager::GetInstance().openedScene)
            openedSceneId = SceneManager::GetInstance().openedScene->sceneId;

        nlohmann::json projectJson = Utils::CreateProjectJson(SceneManager::GetInstance().scenes, openedSceneId);
        AssetSaver::SaveProjectToFile(projectFile, projectJson);

        WindowAllScenes::GetInstance().pendingDelete = false;
    } 
}

/*
PURPOSE: Performs delete actions for tabs
    This function is called by PerformDeleteActions
*/
void Engine::PerformTabDeletion(std::string& projectDir, std::string& projectFile)
{
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
                VisualScriptManager::GetInstance().CloseScript(openedScriptId, projectDir, InterfaceManager::GetInstance().tabs);
            }
        }
        InterfaceManager::GetInstance().pendingTabDelete = false;
    }
}

/*
PURPOSE: Handles opening or creation part of the project
*/
void Engine::HandleProjectOpeningOrCreation()
{
    if (WindowProjectDialog::GetInstance().isCreatingProject ||
        WindowProjectDialog::GetInstance().isOpeningProject ||
        WindowProjectDialog::GetInstance().isOpeningFromLatestProjects) {

        //Project creation
        if (!HandleProjectCreation())
            return;

        //Project opening with path
        if (!HandleProjectOpeningWithPath())
            return;
        
        //Project opening with latest projects
        if (!HandleProjectOpeningWithLatestProjects())
            return;
    }
}

/*
PURPOSE: Handles creation part of the project
    This function is called by HandleProjectOpeningOrCreation
*/
bool Engine::HandleProjectCreation()
{
    if (WindowProjectDialog::GetInstance().isCreatingProject) {
        WindowProjectDialog::GetInstance().isCreatingProject = false;

        if (!Project::Get().CreateProject(WindowProjectDialog::GetInstance().createPath, WindowProjectDialog::GetInstance().createProjectName, window, context, nodesContext, entityTypes)) {
            WindowModalDialog::GetInstance().ShowModalAlert("Creating Project Error", "Couln't create this project!");
            return false;
        }
    }

    return true;
}

/*
PURPOSE: Handles "opening with path" part of the project
    This function is called by HandleProjectOpeningOrCreation
*/
bool Engine::HandleProjectOpeningWithPath()
{
    if (WindowProjectDialog::GetInstance().isOpeningProject) {
        WindowProjectDialog::GetInstance().isOpeningProject = false;

        if (!Project::Get().OpenProject(WindowProjectDialog::GetInstance().openPath, WindowProjectDialog::GetInstance().openProjectName, window, context, nodesContext, entityTypes)) {
            WindowModalDialog::GetInstance().ShowModalAlert("Loading Project Error", "Couln't load this project!");
            return false;
        }
    }

    return true;
}

/*
PURPOSE: Handles "opening with latest project" part of the project
    This function is called by HandleProjectOpeningOrCreation
*/
bool Engine::HandleProjectOpeningWithLatestProjects()
{
    if (WindowProjectDialog::GetInstance().isOpeningFromLatestProjects) {
        WindowProjectDialog::GetInstance().isOpeningFromLatestProjects = false;

        if (!Project::Get().OpenProject(WindowProjectDialog::GetInstance().openLatestPath, WindowProjectDialog::GetInstance().openLatestProjectName, window, context, nodesContext, entityTypes)) {
            WindowModalDialog::GetInstance().ShowModalQuestion(
                "Loading Project Error",
                "Couln't load this project! Would you like to delete it from latest projects?",
                []() {
                    Project::Get().RemoveProjectFromLatestProjects(Project::Get().GetProjectFileLocation());
                    Project::Get().SaveLatestProjects();
                }
            );
            return false;
        }
    }

    return true;
}

/*
PURPOSE: To get the instance of the engine
*/
Engine& Engine::GetInstance()
{
    static Engine engine;
    return engine;
}
