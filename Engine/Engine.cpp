#include "Project.h"
#include "pch.h"
#include "Engine.h"

/*
This is for including all window classes,
because MenuBar includes all window classes,
so we don't need to include them one by one for now
This will be changed later
*/
#include "MenuBar.h"

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

    //Initialize managers
    ServiceLocator::Register<IProject>(&Project::GetInstance());
	ServiceLocator::Register<ISceneManager>(&SceneManager::GetInstance());
	ServiceLocator::Register<IVisualScriptManager>(&VisualScriptManager::GetInstance());

    //Load latest projects
    ServiceLocator::Get<IProject>()->LoadLatestProjects();

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
    if (ServiceLocator::Get<IProject>()->GetProjectOpened()) {
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

    InterfaceManager::GetInstance().DrawInterface();

    InterfaceManager::GetInstance().EndFrame();

    InterfaceManager::GetInstance().UpdateViewportContext();
}

/*
PURPOSE: Close all engines include this one
*/
void Engine::CloseEngine()
{
    //Close Project if it is opened
    if (ServiceLocator::Get<IProject>()->GetProjectOpened()) {
        InterfaceManager::GetInstance().ResetInterface();
        ServiceLocator::Get<IProject>()->CloseProject();
    }

    //Release other engines
    InterfaceManager::GetInstance().CloseInterface();
    Graphics::GetInstance().ReleaseGraphics();

    //Release entity types
    entityTypes.clear();

    //Log
    Logger::Log("P", "Cleared engine");
}

/*
PURPOSE: Creates a new entitymanager for a scene
*/
ENGINE_API std::shared_ptr<IEntityManager> Engine::CreateEntityManager()
{
    return std::make_shared<EntityManager>();
}

/*
PURPOSE: Creates a new entity from its type
*/
ENGINE_API std::shared_ptr<IEntity> Engine::CreateEntity(std::string entityType)
{
    //Checks for the type
    auto& types = entityTypes;
    auto typeIter = types.find(entityType);
    if (typeIter == types.end())
        return nullptr;

    //Create an entity clone object from entity type
    auto entity = typeIter->second.first->clone();

    //Create parameter object for the entity
    auto params = typeIter->second.second->clone();
    if (!entity->CreateEntity(params))
        return nullptr;

    return entity;
}

/*
PURPOSE: Creates a new scene and returns it as a smart pointer
*/
ENGINE_API std::shared_ptr<IScene> Engine::CreateScene()
{
    std::shared_ptr<IScene> scene = std::make_shared<Scene>();
    return scene;
}

/*
PURPOSE: Creates a physics object and returns it as a smart pointer
*/
ENGINE_API std::shared_ptr<IPhysics> Engine::CreatePhysics()
{
    return std::make_shared<BulletPhysics>();
}

/*
PURPOSE: Creates a rigidbody object and returns it as a smart pointer
*/
ENGINE_API std::shared_ptr<IRigidBody> Engine::CreateRigidBody()
{
    return std::make_shared<RigidBody>();
}

/*
PURPOSE: Returns all types of entities as an unordered_map
*/
ENGINE_API std::unordered_map<std::string, std::pair<std::shared_ptr<IEntity>, std::shared_ptr<IEntityParams>>>& Engine::GetEntityTypes()
{
    return entityTypes;
}

/*
PURPOSE: Returns screen width and height as a std::pair
    first -> width
    second -> height
*/
ENGINE_API std::pair<int, int> Engine::GetScreenSize()
{
    return { screenWidth, screenHeight };
}

/*
PURPOSE: Returns fps and ms as a std::pair
    first -> fps
    second -> ms
*/
ENGINE_API std::pair<float, float> Engine::GetFPSandMS()
{
    return { FPS, ms };
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
    if (SceneManager::GetInstance().GetOpenedScene()) {
        //We will use tileMapBrush when "start drawing" button clicked
        std::shared_ptr<ITileMap> edittingTileMap = WindowTileMapBrush::GetInstance().editing ? WindowTileMapBrush::GetInstance().editingTileMap : nullptr;

        SceneManager::GetInstance().GetOpenedScene()->UpdateScene(
            WindowGameViewport::GetInstance().isPlaying,
            WindowGameViewport::GetInstance().isHovered,
            WindowGameViewport::GetInstance().isFocused,
            (int)WindowGameViewport::GetInstance().window_width,
            (int)WindowGameViewport::GetInstance().window_height,
            WindowScene::GetInstance().focused,
            WindowScene::GetInstance().deletePressed,
            WindowScene::GetInstance().pendingDelete,
            WindowScene::GetInstance().selectedId,
            [](std::string scriptId) {
                //Delete tab
                InterfaceManager::GetInstance().RemoveTab(scriptId);

                //Delete opened script
                auto script = VisualScriptManager::GetInstance().GetOpenedScripts().find(scriptId);
                if (script != VisualScriptManager::GetInstance().GetOpenedScripts().end()) {
                    VisualScriptManager::GetInstance().GetOpenedScripts().erase(script);
                }

                //Select nothing
                WindowEntityProperties::GetInstance().SelectEntity(nullptr);
            });
    }
}

/*
PURPOSE: Performs delete actions like deleting scene
*/
void Engine::PerformDeleteActions()
{
    //perform deleting scene
    PerformSceneDeletion();

    //perform deleting tab
    PerformTabDeletion();
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
void Engine::PerformSceneDeletion()
{
    if (WindowAllScenes::GetInstance().pendingDelete) {
        SceneManager::GetInstance().DeleteScene(WindowAllScenes::GetInstance().selectedSceneId);

        //Clear all tabs
		InterfaceManager::GetInstance().RemoveAllTabs();

        //Save project
        std::string openedSceneId = "";
        if (SceneManager::GetInstance().GetOpenedScene())
            openedSceneId = SceneManager::GetInstance().GetOpenedScene()->GetSceneId();

        nlohmann::json projectJson = Utils::CreateProjectJson(SceneManager::GetInstance().GetScenes(), openedSceneId);
        AssetSaver::SaveProjectToFile(ServiceLocator::Get<IProject>()->GetProjectDir(), projectJson);

        WindowAllScenes::GetInstance().pendingDelete = false;
    } 
}

/*
PURPOSE: Performs delete actions for tabs
    This function is called by PerformDeleteActions
*/
void Engine::PerformTabDeletion()
{
    if (InterfaceManager::GetInstance().GetPendingTabDelete()) {
		auto tab = InterfaceManager::GetInstance().GetDeletingTab();

        if (tab) {
            //if tabType is scene
            if (tab->tabType == Utils::SceneEditor) {
                SceneManager::GetInstance().CloseScene(tab->id);

                //Remove the tab from tabs
                InterfaceManager::GetInstance().RemoveAllTabs();

                //Save project
                std::string openedSceneId = "";
                if (SceneManager::GetInstance().GetOpenedScene())
                    openedSceneId = SceneManager::GetInstance().GetOpenedScene()->GetSceneId();

                nlohmann::json projectJson = Utils::CreateProjectJson(SceneManager::GetInstance().GetScenes(), openedSceneId);
                AssetSaver::SaveProjectToFile(ServiceLocator::Get<IProject>()->GetProjectFileLocation(), projectJson);
            }
            //if tabType is visualscript
            else if (tab->tabType == Utils::VisualScriptEditor) {
                std::string openedScriptId = tab->id;
                std::string oldScriptId = "";

                if (VisualScriptManager::GetInstance().GetCurrentScript())
                    oldScriptId = VisualScriptManager::GetInstance().GetCurrentScript()->GetScriptId();

                if (VisualScriptManager::GetInstance().GetCurrentScript() && oldScriptId == openedScriptId) {
                    VisualScriptManager::GetInstance().SetCurrentScript(nullptr);

					InterfaceManager::GetInstance().ActivateTab("");
                }

                VisualScriptManager::GetInstance().CloseScript(openedScriptId);

				//Remove the tab from tabs
				InterfaceManager::GetInstance().RemoveTab(openedScriptId);
            }
        }
		InterfaceManager::GetInstance().SetPendingTabDelete(false);
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

        //Add a new sceneeditortab
        if (ServiceLocator::Get<ISceneManager>()->GetOpenedScene()) {
            InterfaceManager::GetInstance().AddTab(ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetSceneId(), Utils::SceneEditor);
            InterfaceManager::GetInstance().ActivateTab(ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetSceneId());
        }
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

        if (!ServiceLocator::Get<IProject>()->CreateProject(WindowProjectDialog::GetInstance().createPath, WindowProjectDialog::GetInstance().createProjectName, window, context, nodesContext)) {
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

        if (!ServiceLocator::Get<IProject>()->OpenProject(WindowProjectDialog::GetInstance().openPath, WindowProjectDialog::GetInstance().openProjectName, window, context, nodesContext)) {
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

        if (!ServiceLocator::Get<IProject>()->OpenProject(WindowProjectDialog::GetInstance().openLatestPath, WindowProjectDialog::GetInstance().openLatestProjectName, window, context, nodesContext)) {
            WindowModalDialog::GetInstance().ShowModalQuestion(
                "Loading Project Error",
                "Couln't load this project! Would you like to delete it from latest projects?",
                []() {
                    ServiceLocator::Get<IProject>()->RemoveProjectFromLatestProjects(ServiceLocator::Get<IProject>()->GetProjectFileLocation());
                    ServiceLocator::Get<IProject>()->SaveLatestProjects();
                }
            );
            return false;
        }
    }

    return true;
}