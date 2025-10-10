#include "pch.h"
#include "Project.h"

/*
PURPOSE: To load existing project
*/
PROJECT_API bool Project::OpenProject(
    std::string& projectPath,
    std::string& projectName,
    GLFWwindow* window,
    ImGuiContext* context,
    ImNodesContext* nodesContext)
{
    /*
        Setup project specifications.
        This process was applying again in CreateProject function.
        Two functions can be used separately. So we are doing this process.
    */
    this->projectName = projectName;
    this->projectPath = projectPath;
    projectDir = projectPath + projectName + "\\";
    projectFileLocation = projectDir + projectName + ".adrengineproject";
    std::string projectFile = GetProjectFileLocation();

    //set up the project file and load it
    std::ifstream file(projectFile);

    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading project asset.");
        return false;
    }

    nlohmann::json projectJson;
    file >> projectJson;
    file.close();

    //loads asset database
    AssetDatabase::GetInstance().LoadDatabase(projectDir + "asset_database.adrenginedatabase");

    /* Start engines */

    //input manager
    if (!InputManager::GetInstance().InitEngine(window, context))
        return false;

    //scene manager
    if (!ServiceLocator::Get<ISceneManager>()->InitializeManager())
        return false;

    //visual script manager
    if (!ServiceLocator::Get<IVisualScriptManager>()->InitManager(context, nodesContext))
        return false;

    //loads all scenes and scripts that belong to the project
    for (auto& scene : projectJson["scenes"]) {
        ServiceLocator::Get<ISceneManager>()->GetScenes().insert(std::pair<std::string, std::string>(scene, scene));
    }

    if (projectJson.contains("opened-scene")) {
        //it is a scene so load the scene
        std::shared_ptr<IScene> scene = ServiceLocator::Get<ISceneManager>()->LoadScene(projectJson["opened-scene"]);
        if (!scene)
            return false;
    }

    //Close project dialog window and set project opened state true
    projectOpened = true;

    //Add the project to latest projects
    AddProjectToLatestProjects(GetProjectFileLocation());

    //Save latest projects
    SaveLatestProjects();

    return true;
}

/*

PURPOSE: To save the project

*/
bool Project::SaveProject() {
    //get scene id
    std::string sceneId = "";
    if (ServiceLocator::Get<ISceneManager>()->GetOpenedScene())
        sceneId = ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetSceneId();

    //saves the project to the project file
    std::string projectFile = GetProjectFileLocation();
    std::string projectDir = GetProjectDir();
    nlohmann::json projectJson = Utils::CreateProjectJson(ServiceLocator::Get<ISceneManager>()->GetScenes(), sceneId);
    AssetSaver::SaveProjectToFile(projectFile, projectJson);

    //save assets
    AssetDatabase::GetInstance().SaveDatabase(projectDir + "asset_database.adrenginedatabase");

    //saves each opened-scenes and each entity that belong to the scene
    std::shared_ptr<IScene> scene = ServiceLocator::Get<ISceneManager>()->GetOpenedScene();
    if (scene) {
        AssetSaver::SaveSceneToFile(scene->ToJson(), projectDir, sceneId);

        if (scene->GetEntityManager()) {
            for (auto& entity : scene->GetEntityManager()->GetEntities()) {
                AssetSaver::SaveEntityToFile(entity.second->ToJson(), projectDir, entity.second->GetEntityParams()->GetId());
            }
        }
    }

    //saves each opened-scripts
    for (auto& scriptIter : ServiceLocator::Get<IVisualScriptManager>()->GetOpenedScripts()) {
        auto script = scriptIter.second.get();
        AssetSaver::SaveScriptToFile(script->ToJson(), projectDir, script->GetScriptId());
    }

    return true;
}

/*

PURPOSE: Creates a folder for the project

*/
bool Project::CreateProject(
    std::string projectPath,
    std::string projectName,
    GLFWwindow* window,
    ImGuiContext* context,
    ImNodesContext* nodesContext)
{
    //Setup some variables
    this->projectName = projectName;
    this->projectPath = projectPath;
    projectDir = projectPath + projectName + "\\";
    projectFileLocation = projectDir + projectName + ".adrengineproject";

    //Create a folder for the project
    if (!std::filesystem::create_directory(projectDir))
        return false;

    //Call this to create first files
    if (!SaveProject())
        return false;

    //Open the project
    if (!OpenProject(projectPath, projectName, window, context, nodesContext))
        return false;

    return true;
}

/*

PURPOSE: Closes the project

*/
bool Project::CloseProject() {
    InputManager::GetInstance().ReleaseEngine();
    ServiceLocator::Get<ISceneManager>()->ClearManager();
    ServiceLocator::Get<IVisualScriptManager>()->ReleaseManager();

    projectOpened = false;
    return true;
}

/*

PURPOSE: Loads latest project file paths

*/
void Project::LoadLatestProjects()
{
    //Open the latest project file
    std::ifstream file("latestprojects.adrengineprojects");

    //Check if the file exists
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading latest projects");
        return;
    }

    //Load latest project file paths by reading line by line
    std::string projectFilePath;
    while (std::getline(file, projectFilePath)) {
        //Add the project
        AddProjectToLatestProjects(projectFilePath);
    }

    //Close the file
    file.close();
}

/*

PURPOSE: Adds a project path to latest projects

*/
PROJECT_API void Project::AddProjectToLatestProjects(std::string& projectFilepath)
{
    //Check if the project was added before
    for (int i = 0; i < latestProjects.size(); ++i) {
        if (latestProjects[i] == projectFilepath) {
            //This project was added before, so we don't have to add it again
            MoveProjectToBeginByIndex(i, projectFilepath); 
            return;
        }
    }

    latestProjects.push_back(projectFilepath);
}

/*

PURPOSE: Remove a project path from latest projects

*/
PROJECT_API void Project::RemoveProjectFromLatestProjects(std::string& projectFilepath)
{
    //Basic vector erasing
    for (int i = 0; i < latestProjects.size(); ++i) {
        if (latestProjects[i] == projectFilepath) {
            RemoveProjectFromLatestProjectsByIndex(i);
            break;
        }
    }
}

/*

PURPOSE: Removes a project path from latest projects by index

*/
PROJECT_API void Project::RemoveProjectFromLatestProjectsByIndex(int& index)
{
    if(latestProjects.size() > index)
        latestProjects.erase(latestProjects.begin() + index);
}

/*

PURPOSE: Moves project path to begin of latest projects

*/
PROJECT_API void Project::MoveProjectToBegin(std::string& projectFilepath)
{
    for (int i = 0; i < latestProjects.size(); ++i) {
        if (latestProjects[i] == projectFilepath) {
            RemoveProjectFromLatestProjectsByIndex(i); //Remove it by index
        }
    }

    //Add it to begin
    latestProjects.insert(latestProjects.begin(), projectFilepath);
}

/*

PURPOSE: Moves project path that has given index to begin of latest projects

*/
PROJECT_API void Project::MoveProjectToBeginByIndex(int& index, std::string& projectFilepathWillBeAdded)
{
    RemoveProjectFromLatestProjectsByIndex(index); //Remove it by index

    //Add it to begin
    latestProjects.insert(latestProjects.begin(), projectFilepathWillBeAdded);
}

/*

PURPOSE: Saves latest projects file paths

*/
PROJECT_API void Project::SaveLatestProjects()
{
    //Open the latest project file
    std::ofstream file("latestprojects.adrengineprojects");

    //Check if the file exists
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving latest projects");
        return;
    }

    //Save latest project file paths line by line
    for (auto& project : latestProjects) {
        file << project << std::endl;
    }

    //Close the file
    file.close();
}

/*

PURPOSE: Returns latest projects

*/
PROJECT_API std::vector<std::string>& Project::GetLatestProjects()
{
    return latestProjects;
}

/*

PURPOSE: Returns main file location of the project

*/
std::string& Project::GetProjectFileLocation() {
    return projectFileLocation;
}

/*

PURPOSE: Returns directory of the project

*/
std::string& Project::GetProjectDir() {
    return projectDir;
}

/*

PURPOSE: Returns true if a project is opened, otherwise returns false

*/
bool Project::GetProjectOpened() {
    return projectOpened;
}