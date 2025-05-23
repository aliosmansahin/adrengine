#include "pch.h"
#include "VisualScriptManager.h"

//Tab type
enum TabType {
    SceneEditor,
    VisualScriptEditor
};
struct Tab {
    std::string id;
    TabType tabType;
};

/*
TODO: This will be moved to another place
*/
nlohmann::json CreateProjectJson(
    std::string openedTabId,
    std::map<std::string, std::string>& scenes,
    std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts,
    std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs) {

    nlohmann::json projectJson;

    projectJson["current-tab"] = openedTabId;

    for (auto& sceneIter : scenes) {
        auto& scene = sceneIter.first;
        projectJson["scenes"].push_back(scene);
    }
    for (auto& scriptIter : scripts) {
        auto& script = scriptIter.first;
        projectJson["scripts"].push_back(script);
    }
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
PURPOSE: Initializes the manager
*/
bool VisualScriptManager::InitManager()
{
    //Create all node types
    types = { {
        {"Begin", std::make_shared<Begin>()},
        {"Return", std::make_shared<Return>()},
        {"Print", std::make_shared<Print>()}
    } };
	return true;
}

/*
PURPOSE: Releases all visual script stuff
*/
void VisualScriptManager::ReleaseManager()
{
    //currentScript is nullptr anymore
    currentScript = nullptr;

    //Clear script "unordered_map"s
    scripts.clear();
    openedScripts.clear();
    types.clear();

    //logger
    Logger::Log("P", "Cleared visual script manager");
}

/*
PURPOSE: Opens the script to edit
*/
std::pair<std::shared_ptr<VisualScript>, std::shared_ptr<Tab>> VisualScriptManager::OpenScript(std::string scriptId, std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs)
{
    //If the script or tab is opened, interrupt the function
    auto openedScriptIter = openedScripts.find(scriptId);
    if (openedScriptIter != openedScripts.end()) {
        return {nullptr, nullptr};
    }
    auto tabIter = tabs.find(scriptId);
    if (tabIter != tabs.end()) {
        return { nullptr, nullptr };
    }

    //If the script doesn't exists in all scripts
    auto scriptIter = scripts.find(scriptId);
    if (scriptIter == scripts.end())
        return { nullptr, nullptr };

    //Open the script
    std::shared_ptr<VisualScript> script = scriptIter->second;
    openedScripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(script->scriptId, script));

    //Create a tab and insert it to tabs
    std::shared_ptr<Tab> tab = std::make_shared<Tab>();
    tab->id = scriptId;
    tab->tabType = VisualScriptEditor;
    tabs.insert(std::pair<std::string, std::shared_ptr<Tab>>(tab->id, tab));

    //return the script and tab as a pair
    return { script, tab };
}

/*
PURPOSE: Loads the script to run
*/
bool VisualScriptManager::LoadScript(std::string scriptId, std::string& projectDir, std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs)
{
    //If the script or tab is opened, interrupt the function
    auto openedScriptIter = scripts.find(scriptId);
    if (openedScriptIter != scripts.end()) {
        return false;
    }
    auto tabIter = tabs.find(scriptId);
    if (tabIter != tabs.end()) {
        return false;
    }

    //Load the script
    //std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scriptsDir = projectDir + "scripts/";
    std::filesystem::create_directory(scriptsDir);
    std::string scriptDir = scriptsDir + scriptId + "/";
    std::filesystem::create_directory(scriptDir);
    std::string scriptFile = scriptDir + scriptId + ".adrenginescript";

    nlohmann::json scriptJson = AssetSaver::LoadScriptFromFile(scriptFile);

    //Create a script object and insert it to scripts
    VisualScript* script = new VisualScript();
    script->FromJson(scriptJson, types);
    if (!script)
        return false;

    scripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(scriptId, std::shared_ptr<VisualScript>(script)));

    return true;
}

/*
PURPOSE: Saves and closes the script
*/
bool VisualScriptManager::CloseScript(
    std::string scriptId,
    std::string& projectDir,
    std::unordered_map<std::string,
    std::shared_ptr<Tab>>& tabs,
    Tab*& openedTab,
    std::map<std::string, std::string>& scenes)
{
    //If the tab doesn't exist, interrupt the function
    auto tab = tabs.find(scriptId);

    if (tab == tabs.end()) {
        return false;
    }

    //Check if script is opened
    auto openedScript = openedScripts.find(scriptId);
    if (openedScript == openedScripts.end()) {
        //This script is not opened
        std::string str = "There is not any script which has given id \"";
        str += scriptId;
        str += "\"";
        Logger::Log("E", str.c_str());
        return false;
    }
    else {
        //This script is opened

        auto script = openedScript->second.get();

        //std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
        //Save the script
        std::string scriptsDir = projectDir + "scripts/";
        std::filesystem::create_directory(scriptsDir);
        std::string scriptDir = scriptsDir + scriptId + "/";
        std::filesystem::create_directory(scriptDir);
        std::string scriptFile = scriptDir + scriptId + ".adrenginescript";
        AssetSaver::SaveScriptToFile(script->ToJson(), scriptFile);

        //Remove the tab and script from tabs and openedScripts
        tabs.erase(tab);
        openedScripts.erase(openedScript);

        //Save the project
        std::string projectFile = projectDir + "project.adrengineproject";

        std::string openedTabId = "";

        if (openedTab) {
            openedTabId = openedTab->id;
        }

        nlohmann::json projectJson = CreateProjectJson(openedTabId, scenes, scripts, tabs);
        AssetSaver::SaveProjectToFile(projectFile, projectJson);
    }
	return true;
}

/*
PURPOSE: Creates a new script
*/
bool VisualScriptManager::CreateScript(ScriptBelongsTo sbt, std::string& projectDir, std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
    Tab*& openedTab, std::string& selectedTabId, std::map<std::string, std::string>& scenes)
{
    //Set the id
    int index = 0;
    std::string scriptId;
    while (true) {
        scriptId = "script" + std::to_string(index);
        if (scripts.find(scriptId) == scripts.end())
            break;
        ++index;
    }
    
    //TODO: write scene codes

    std::string sceneId = sbt.sceneJson.value("id", "");
    if (sceneId == "")
        return false;


    std::shared_ptr<VisualScript> script = std::make_shared<VisualScript>();
    if (sceneId != "") {
        if (sbt.entity) {//this script belogns to an entity
            script->CreateScript(scriptId, sbt.entity->GetEntityParams()->id, sceneId);
            sbt.entity->GetEntityParams()->script = script.get();
            sbt.entity->GetEntityParams()->scriptId = scriptId;
        }
        else // this script belongs to a scene
            script->CreateScript(scriptId, sceneId, sceneId);
    }
    else
        return false;

    //Insert the script to "unordered_map"s
    scripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(scriptId, script));
    openedScripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(scriptId, script));

    //Save the script
    //std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";

    //to save script folder
    std::string scriptsDir = projectDir + "scripts/";
    std::filesystem::create_directory(scriptsDir);
    std::string scriptDir = scriptsDir + scriptId + "/";
    std::filesystem::create_directory(scriptDir);
    std::string scriptFile = scriptDir + scriptId + ".adrenginescript";
    AssetSaver::SaveScriptToFile(script->ToJson(), scriptFile);

    
    //Save the scene
    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string sceneDir = scenesDir + sceneId + "/";
    std::filesystem::create_directory(sceneDir);
    std::string sceneFile = sceneDir + sceneId + ".adrenginescene";
    AssetSaver::SaveSceneToFile(sbt.sceneJson, sceneFile, projectDir);

    //Open a new tab and insert it to tabs
    Tab* tab = new Tab();
    tab->id = scriptId;
    tab->tabType = VisualScriptEditor;

    tabs.insert(std::pair<std::string, std::unique_ptr<Tab>>(tab->id, std::unique_ptr<Tab>(tab)));

    //Set the currents
    currentScript = script;
    openedTab = tab;
    selectedTabId = tab->id;

    //Save the project
    std::string projectFile = projectDir + "project.adrengineproject";

    nlohmann::json projectJson = CreateProjectJson(openedTab->id, scenes, scripts, tabs);

    AssetSaver::SaveProjectToFile(projectFile, projectJson);
    return true;
}

/*
PURPOSE: Deletes the script
*/
bool VisualScriptManager::DeleteScript(std::string scriptId, ScriptBelongsTo sbt)
{
    std::string sbtStr = "";
    if (sbt.entity) {
        sbt.entity->GetEntityParams()->scriptId = scriptId;
        sbtStr = "entity";
    }
    
    //TODO: Write deleting script codes

    std::string str = "Deleted script \"";
    str += scriptId;
    str += "\"";
    Logger::Log("P", str.c_str());
	return true;
}

/*
PURPOSE: Saves the script
*/
bool VisualScriptManager::SaveScript(std::shared_ptr<VisualScript> script, std::string& projectDir,
    Tab*& openedTab,
    std::map<std::string, std::string>& scenes,
    std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs)
{
    //std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";

    //Saves the script
    std::string scriptsDir = projectDir + "scripts/";
    std::filesystem::create_directory(scriptsDir);
    std::string scriptDir = scriptsDir + script->scriptId + "/";
    std::filesystem::create_directory(scriptDir);
    std::string scriptFile = scriptDir + script->scriptId + ".adrenginescript";
    AssetSaver::SaveScriptToFile(script->ToJson(), scriptFile);

    //Saves the project
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
PURPOSE: Runs all begin scripts
*/
void VisualScriptManager::RunScriptsBegin()
{
    for (auto& script : scripts) {
        auto& compiled = script.second->compiled;
        for (auto& compile : compiled) {
            compile();
        }
    }
}

/*
PURPOSE: Gets the instance of the class
*/
VisualScriptManager& VisualScriptManager::GetInstance()
{
	static VisualScriptManager manager;
	return manager;
}
