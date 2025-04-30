#include "VisualScriptManager.h"

bool VisualScriptManager::InitManager()
{
    types = { {
        {"Begin", std::make_shared<Begin>()},
        {"Return", std::make_shared<Return>()},
        {"Print", std::make_shared<Print>()}
    } };
	return true;
}

void VisualScriptManager::ReleaseManager()
{
    scripts.clear();
    openedScripts.clear();
    
    Logger::Log("P", "Cleared visual script manager");
}

std::pair<std::shared_ptr<VisualScript>, std::shared_ptr<Tab>> VisualScriptManager::OpenScript(std::string scriptId)
{
    auto openedScriptIter = openedScripts.find(scriptId);
    if (openedScriptIter != openedScripts.end()) {
        return {nullptr, nullptr};
    }
    auto tabIter = InterfaceManager::GetInstance().tabs.find(scriptId);
    if (tabIter != InterfaceManager::GetInstance().tabs.end()) {
        return { nullptr, nullptr };
    }

    auto scriptIter = scripts.find(scriptId);
    if (scriptIter == scripts.end())
        return { nullptr, nullptr };

    std::shared_ptr<VisualScript> script = scriptIter->second;

    openedScripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(script->scriptId, script));

    std::shared_ptr<Tab> tab = std::make_shared<Tab>();
    tab->id = scriptId;
    tab->tabType = VisualScriptEditor;

    InterfaceManager::GetInstance().tabs.insert(std::pair<std::string, std::shared_ptr<Tab>>(tab->id, tab));

    return { script, tab };
}

bool VisualScriptManager::LoadScript(std::string scriptId)
{
    auto openedScriptIter = scripts.find(scriptId);
    if (openedScriptIter != scripts.end()) {
        return false;
    }
    auto tabIter = InterfaceManager::GetInstance().tabs.find(scriptId);
    if (tabIter != InterfaceManager::GetInstance().tabs.end()) {
        return false;
    }
    std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
    std::string scriptsDir = projectDir + "scripts/";
    std::filesystem::create_directory(scriptsDir);
    std::string scriptDir = scriptsDir + scriptId + "/";
    std::filesystem::create_directory(scriptDir);
    std::string scriptFile = scriptDir + scriptId + ".adrenginescript";

    VisualScript* script = new VisualScript();
    script = AssetSaver::LoadScriptFromFile(scriptFile);
    if (!script)
        return false;

    scripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(scriptId, std::shared_ptr<VisualScript>(script)));

    return true;
}

bool VisualScriptManager::CloseScript(std::string scriptId)
{
    auto tab = InterfaceManager::GetInstance().tabs.find(scriptId);

    if (tab == InterfaceManager::GetInstance().tabs.end()) {
        return false;
    }

    auto openedScript = openedScripts.find(scriptId);
    if (openedScript == openedScripts.end()) {
        std::string str = "There is not any script which has given id \"";
        str += scriptId;
        str += "\"";
        Logger::Log("E", str.c_str());
        return false;
    }
    else {
        auto script = openedScript->second.get();

        std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";

        //to save script folder
        std::string scriptsDir = projectDir + "scripts/";
        std::filesystem::create_directory(scriptsDir);
        std::string scriptDir = scriptsDir + scriptId + "/";
        std::filesystem::create_directory(scriptDir);
        std::string scriptFile = scriptDir + scriptId + ".adrenginescript";
        AssetSaver::SaveScriptToFile(script, scriptFile);

        InterfaceManager::GetInstance().tabs.erase(tab);
        openedScripts.erase(openedScript);

        std::string projectFile = projectDir + "project.adrengineproject";

        AssetSaver::SaveProjectToFile(projectFile);
    }
	return true;
}

bool VisualScriptManager::CreateScript(ScriptBelongsTo sbt)
{
    int index = 0;
    std::string scriptId;
    while (true) {
        scriptId = "script" + std::to_string(index);
        if (scripts.find(scriptId) == scripts.end())
            break;
        ++index;
    }//TODO: write scene codes

    std::shared_ptr<VisualScript> script = std::make_shared<VisualScript>();
    if (sbt.scene) {
        if (sbt.entity) {//this script belogns to an entity
            script->CreateScript(scriptId, sbt.entity->GetEntityParams()->id, sbt.scene->sceneId);
            sbt.entity->GetEntityParams()->script = script.get();
            sbt.entity->GetEntityParams()->scriptId = scriptId;
        }
        else // this script belongs to a scene
            script->CreateScript(scriptId, sbt.scene->sceneId, sbt.scene->sceneId);
    }
    else
        return false;

    scripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(scriptId, script));
    openedScripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(scriptId, script));

    std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";

    //to save script folder
    std::string scriptsDir = projectDir + "scripts/";
    std::filesystem::create_directory(scriptsDir);
    std::string scriptDir = scriptsDir + scriptId + "/";
    std::filesystem::create_directory(scriptDir);
    std::string scriptFile = scriptDir + scriptId + ".adrenginescript";
    AssetSaver::SaveScriptToFile(script.get(), scriptFile);
    
    //to save scene folder
    std::string scenesDir = projectDir + "scenes/";
    std::filesystem::create_directory(scenesDir);
    std::string sceneDir = scenesDir + sbt.scene->sceneId + "/";
    std::filesystem::create_directory(sceneDir);
    std::string sceneFile = sceneDir + sbt.scene->sceneId + ".adrenginescene";
    AssetSaver::SaveSceneToFile(sbt.scene, sceneFile);

    //opening a new tab
    Tab* tab = new Tab();
    tab->id = scriptId;
    tab->tabType = VisualScriptEditor;

    InterfaceManager::GetInstance().tabs.insert(std::pair<std::string, std::unique_ptr<Tab>>(tab->id, std::unique_ptr<Tab>(tab)));

    currentScript = script;
    InterfaceManager::GetInstance().openedTab = tab;
    InterfaceManager::GetInstance().selectedTabId = tab->id;

    //to save project file
    std::string projectFile = projectDir + "project.adrengineproject";
    AssetSaver::SaveProjectToFile(projectFile);
    return true;
}

bool VisualScriptManager::DeleteScript(std::string scriptId, ScriptBelongsTo sbt)
{
    std::string sbtStr = "";
    if (sbt.entity) {
        sbt.entity->GetEntityParams()->scriptId = scriptId;
        sbtStr = "entity";
    }

    std::string str = "Deleted script \"";
    str += scriptId;
    str += "\"";
    Logger::Log("P", str.c_str());
	return true;
}

bool VisualScriptManager::SaveScript(std::shared_ptr<VisualScript> script)
{
    std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";

    //to save script folder
    std::string scriptsDir = projectDir + "scripts/";
    std::filesystem::create_directory(scriptsDir);
    std::string scriptDir = scriptsDir + script->scriptId + "/";
    std::filesystem::create_directory(scriptDir);
    std::string scriptFile = scriptDir + script->scriptId + ".adrenginescript";
    AssetSaver::SaveScriptToFile(script.get(), scriptFile);

    std::string projectFile = projectDir + "project.adrengineproject";

    AssetSaver::SaveProjectToFile(projectFile);
    return true;
}

std::string VisualScriptManager::CompileScript(VisualScript* script)
{
    auto& nodes = script->nodes;
    auto& links = script->links;

    std::unordered_map<int, int> organizedNodes;

    auto begin = nodes.find(0);
    if (begin == nodes.end())
        return "Compile: Begin Entrypoint could not found";

    auto returnNode = nodes.find(2);
    if (returnNode == nodes.end())
        return "Compile: Return could not found";

    bool returnConnected = false;
    for (auto& link : links) {
        int first = link.second.first;
        int second = link.second.second;

        auto beforeNode = nodes.find(first - 1); //-1 output //-2 input
        auto afterNode = nodes.find(second - 2);

        organizedNodes.insert(std::pair<int, int>(beforeNode->first, afterNode->first));
        if (second == 2 + 2) { //Return Id 
            returnConnected = true;
        }
    }

    int id = 0;
    std::unordered_map<int, int>::iterator organizedNode = organizedNodes.find(id);
    if (organizedNode == organizedNodes.end())
        return "Compile: Begin Entrypoint wasn't connected to any node";

    if (!returnConnected)
        return "Compile: Return wasn't connected to any node";

    script->compiled.clear();

    while (true) {
        auto nodeIter = nodes.find(organizedNode->second);
        if (nodeIter->second->GetType() == "Return")
            break;
        script->compiled.push_back(nodeIter->second->Compile());
        organizedNode = organizedNodes.find(organizedNode->second);
    }

    return "";
}

void VisualScriptManager::RunScriptsBegin()
{
    for (auto& script : scripts) {
        auto& compiled = script.second->compiled;
        for (auto& compile : compiled) {
            compile();
        }
    }
}

VisualScriptManager& VisualScriptManager::GetInstance()
{
	static VisualScriptManager manager;
	return manager;
}
