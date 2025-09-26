#include "pch.h"
#include "Project.h"

/*
PURPOSE: To load existing project
*/
PROJECT_API bool Project::OpenProject(std::string& projectPath, std::string& projectName,
    std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes)
{
    //Setup project specifications
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

    //loads all scenes and scripts that belong to the project
    for (auto& scene : projectJson["scenes"]) {
        SceneManager::GetInstance().scenes.insert(std::pair<std::string, std::string>(scene, scene));
    }

    if (projectJson.contains("opened-scene")) {
        //it is a scene so load the scene
        Scene* scene = SceneManager::GetInstance().LoadScene(projectJson["opened-scene"], projectDir, entityTypes);
        if (!scene)
            return false;

        //Create a tab and insert it to tabs
        std::shared_ptr<Utils::Tab> tab = std::make_shared<Utils::Tab>();
        tab->id = scene->sceneId;
        tab->tabType = Utils::SceneEditor;
        InterfaceManager::GetInstance().tabs.insert(std::pair<std::string, std::shared_ptr<Utils::Tab>>(tab->id, tab));

        InterfaceManager::GetInstance().openedTab = tab.get();
        InterfaceManager::GetInstance().selectedTabId = tab->id;
    }

    return true;
}

/*

PURPOSE: To save the project

*/
bool Project::SaveProject() {
    //get scene id
    std::string sceneId = "";
    if (SceneManager::GetInstance().openedScene.get())
        sceneId = SceneManager::GetInstance().openedScene->sceneId;

    //saves the project to the project file
    std::string projectFile = Project::Get().GetProjectFileLocation();
    std::string projectDir = Project::Get().GetProjectDir();
    nlohmann::json projectJson = Utils::CreateProjectJson(SceneManager::GetInstance().scenes, sceneId);
    AssetSaver::SaveProjectToFile(projectFile, projectJson);

    //save assets
    AssetDatabase::GetInstance().SaveDatabase(projectDir + "asset_database.adrenginedatabase");

    //saves each opened-scenes and each entity that belong to the scene
    Scene* scene = SceneManager::GetInstance().openedScene.get();
    if (scene) {
        AssetSaver::SaveSceneToFile(scene->ToJson(), projectDir, sceneId);

        if (scene->GetEntityManager()) {
            for (auto& entity : scene->GetEntityManager()->GetEntities()) {
                AssetSaver::SaveEntityToFile(entity.second->ToJson(), projectDir, entity.second->GetEntityParams()->id);
            }
        }
    }

    //saves each opened-scripts
    for (auto& scriptIter : VisualScriptManager::GetInstance().openedScripts) {
        auto script = scriptIter.second.get();
        AssetSaver::SaveScriptToFile(script->ToJson(), projectDir, script->scriptId);
    }

    return true;
}

/*

PURPOSE: Creates a folder for the project

*/
bool Project::CreateProject(std::string projectPath, std::string projectName,
    std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes) {
    this->projectName = projectName;
    this->projectPath = projectPath;
    projectDir = projectPath + projectName + "\\";
    projectFileLocation = projectDir + projectName + ".adrengineproject";

    if (!std::filesystem::create_directory(projectDir))
        return false;

    if (!SaveProject())
        return false;

    if (!OpenProject(projectPath, projectName, entityTypes))
        return false;

    return true;
}

/*

PURPOSE: Closes the project

*/
bool Project::CloseProject() {
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
        latestProjects.push_back(projectFilePath);
    }

    //Close the file
    file.close();
}

/*

PURPOSE: Adds a project path to latest projects

*/
PROJECT_API void Project::AddProjectToLatestProjects(std::string& projectFilepath)
{
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
            latestProjects.erase(latestProjects.begin() + i);
            break;
        }
    }
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

PURPOSE: Gets the instance of the class

*/
Project& Project::Get() {
    static Project project;
    return project;
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