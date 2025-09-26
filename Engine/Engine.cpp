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

    //load existing project
    //TODO: Project Dialog
    //For test
    //std::string projectName = "project";
    //std::string projectPath = "C:\\Users\\osman\\OneDrive\\Desktop\\"; //This is for mine
    //if (!Project::Get().OpenProject(projectName, projectPath, entityTypes))
    //    return false;

    ////Update physics for once
    //for (auto& entity : SceneManager::GetInstance().openedScene->GetEntityManager()->GetEntities()) {
    //    Object* object = dynamic_cast<Object*>(entity.second.get());
    //    if (object != nullptr) {
    //        SceneManager::GetInstance().openedScene->physics->EndEmulationForRigidBody(object->rigidBody);
    //    }
    //}

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
    if (Project::Get().projectOpened) {
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
    else {
        //TODO: Move to a function
        //Check for creating or opening a project
        if (WindowProjectDialog::GetInstance().isCreatingProject || WindowProjectDialog::GetInstance().isOpeningProject) {
            if (WindowProjectDialog::GetInstance().isCreatingProject) {
                WindowProjectDialog::GetInstance().isCreatingProject = false;

                if (!Project::Get().CreateProject(WindowProjectDialog::GetInstance().createPath, WindowProjectDialog::GetInstance().createProjectName, window, context, nodesContext, entityTypes))
                    return; //TODO: Add Loading error dialog window
            }
            if (WindowProjectDialog::GetInstance().isOpeningProject) {
                WindowProjectDialog::GetInstance().isOpeningProject = false;

                if (!Project::Get().OpenProject(WindowProjectDialog::GetInstance().openPath, WindowProjectDialog::GetInstance().openProjectName, window, context, nodesContext, entityTypes))
                    return; //TODO: Add Loading error dialog window
            }

            //Update physics for once
            if (SceneManager::GetInstance().openedScene && SceneManager::GetInstance().openedScene->GetEntityManager()) {
                for (auto& entity : SceneManager::GetInstance().openedScene->GetEntityManager()->GetEntities()) {
                    Object* object = dynamic_cast<Object*>(entity.second.get());
                    if (object != nullptr) {
                        SceneManager::GetInstance().openedScene->physics->EndEmulationForRigidBody(object->rigidBody);
                    }
                }
            }
        }
    }
}

/*
PURPOSE: To draw main frame
*/
void Engine::Draw()
{
    Graphics::GetInstance().Clear();

    InterfaceManager::GetInstance().StartFrame();

    InterfaceManager::GetInstance().DrawInterface(Project::Get().GetProjectDir(), Project::Get().GetProjectFileLocation(), [this]() { Project::Get().SaveProject(); }, [this]() { Project::Get().CloseProject(); }, entityTypes, Project::Get().GetLatestProjects(), FPS, ms, screenWidth, screenHeight, Project::Get().projectOpened);

    InterfaceManager::GetInstance().EndFrame();

    InterfaceManager::GetInstance().UpdateViewportContext();
}

/*
PURPOSE: Close all engines include this one
*/
void Engine::CloseEngine()
{
    if(Project::Get().projectOpened)
        Project::Get().CloseProject();

    InterfaceManager::GetInstance().CloseInterface();
    Graphics::GetInstance().ReleaseGraphics();
    entityTypes.clear();
    Logger::Log("P", "Cleared engine");
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
PURPOSE: To perform delete actions like deleting scene
*/
ENGINE_API void Engine::PerformDeleteActions()
{
    //perform deleting scene
    std::string projectDir = Project::Get().GetProjectDir();
    std::string projectFile = Project::Get().GetProjectFileLocation();

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
PURPOSE: To get the instance of the engine
*/
Engine& Engine::GetInstance()
{
    static Engine engine;
    return engine;
}
