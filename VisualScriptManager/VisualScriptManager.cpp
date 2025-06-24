#include "pch.h"
#include "VisualScriptManager.h"

/*
PURPOSE: Initializes the manager
*/
bool VisualScriptManager::InitManager(ImGuiContext* imguiContext, ImNodesContext* nodesContext)
{
    //Set imgui context
    ImGui::SetCurrentContext(imguiContext);
    ImNodes::SetCurrentContext(nodesContext);

    //Create all node types
    types = { {
        {"Begin", std::make_shared<Begin>()},
        {"Print", std::make_shared<Print>()},
        {"ConstString", std::make_shared<ConstString>()},
        {"GetThisEntity", std::make_shared<GetThisEntity>()},
        {"GetPositionVector", std::make_shared<GetPositionVector>()},
        {"SeparateVector3", std::make_shared<SeparateVector3>()},
        {"ToString", std::make_shared<ToString>()},
        {"SetPositionVector", std::make_shared<SetPositionVector>()},
        {"MakeVector3", std::make_shared<MakeVector3>()},
        {"ConstFloat", std::make_shared<ConstFloat>()},
        {"AddImpulse", std::make_shared<AddImpulse>()},
        {"CastToObject", std::make_shared<CastToObject>()},
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
    openedScripts.clear();
    types.clear();

    //logger
    Logger::Log("P", "Cleared visual script manager");
}

/*
PURPOSE: Opens the script to edit
*/
std::pair<std::shared_ptr<VisualScript>, std::shared_ptr<Utils::Tab>> VisualScriptManager::OpenScript(std::shared_ptr<VisualScript> source, std::unordered_map<std::string, std::shared_ptr<Utils::Tab>>& tabs)
{
    //If the script or tab is opened, interrupt the function
    auto openedScriptIter = openedScripts.find(source->scriptId);
    auto tabIter = tabs.find(source->scriptId);
    if (openedScriptIter != openedScripts.end() && tabIter != tabs.end()) {
        return { openedScriptIter->second, tabIter->second };
    }

    currentScript = source;

    //Open the script
    openedScripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(source->scriptId, source));

    //Create a tab and insert it to tabs
    std::shared_ptr<Utils::Tab> tab = std::make_shared<Utils::Tab>();
    tab->id = source->scriptId;
    tab->tabType = Utils::VisualScriptEditor;
    tabs.insert(std::pair<std::string, std::shared_ptr<Utils::Tab>>(tab->id, tab));

    //return the script and tab as a pair
    return { source, tab };
}

/*
PURPOSE: Loads the script to run
*/
std::shared_ptr<VisualScript> VisualScriptManager::LoadScript(std::string scriptId, std::string& projectDir, IScene* scene)
{
    //Load the script
    nlohmann::json scriptJson = AssetSaver::LoadScriptFromFile(projectDir, scriptId);

    if (scriptJson.is_null())
        return nullptr;

    //Create a script object and insert it to scripts
    VisualScript* script = new VisualScript();
    script->FromJson(scriptJson, types, scene);

    return std::shared_ptr<VisualScript>(script);
}

/*
PURPOSE: Saves and closes the script
*/
bool VisualScriptManager::CloseScript(
    std::string scriptId,
    std::string& projectDir,
    std::unordered_map<std::string, std::shared_ptr<Utils::Tab>>& tabs)
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

        //Save the script
        AssetSaver::SaveScriptToFile(script->ToJson(), projectDir, script->scriptId);

        //Remove the tab and script from tabs and openedScripts
        tabs.erase(tab);
        openedScripts.erase(openedScript);
    }
	return true;
}

/*
PURPOSE: Creates a new script
*/
std::shared_ptr<VisualScript> VisualScriptManager::CreateScript(Utils::ScriptBelongsTo sbt, std::string& projectDir, std::unordered_map<std::string, std::shared_ptr<Utils::Tab>>& tabs,
    Utils::Tab*& openedTab, std::string& selectedTabId, std::map<std::string, std::string>& scenes)
{
    //Set the id
    int index = 0;
    std::string scriptId;
    while (true) {
        scriptId = "script" + std::to_string(index);

        std::string scriptsDir = projectDir + "scripts/";
        std::string scriptDir = scriptsDir + scriptId + "/";

        if (!std::filesystem::exists(scriptDir))
            break;

        ++index;
    }
    
    std::string entityId = sbt.entityJson["id"];
    std::string sceneId = sbt.sceneJson["id"];

    std::shared_ptr<VisualScript> source = std::make_shared<VisualScript>();

    source->CreateScript(scriptId, entityId, sceneId);
    
    //Insert the script to "unordered_map"s
    openedScripts.insert(std::pair<std::string, std::shared_ptr<VisualScript>>(scriptId, source));

    //Save the script
    AssetSaver::SaveScriptToFile(source->ToJson(), projectDir, scriptId);

    //Save the scene
    AssetSaver::SaveSceneToFile(sbt.sceneJson, projectDir, sceneId);

    //Open a new tab and insert it to tabs
    Utils::Tab* tab = new Utils::Tab();
    tab->id = scriptId;
    tab->tabType = Utils::VisualScriptEditor;

    tabs.insert(std::pair<std::string, std::unique_ptr<Utils::Tab>>(tab->id, std::unique_ptr<Utils::Tab>(tab)));

    //Set the currents
    currentScript = source;
    openedTab = tab;
    selectedTabId = tab->id;

    return source;
}

/*
PURPOSE: Deletes the script
*/
bool VisualScriptManager::DeleteScript(
    VisualScript* script,
    std::unordered_map<std::string,
    std::shared_ptr<Utils::Tab>>&tabs,
    Utils::Tab*& openedTab,
    std::string& projectDir)
{
    //Release script
    script->ReleaseScript();

    //Delete directory of the script
    std::string scriptsDir = projectDir + "scripts/";
    std::string scriptDir = scriptsDir + script->scriptId + "/";

    std::filesystem::remove_all(scriptDir);

    //Remove tab
    auto tab = tabs.find(script->scriptId);
    if (tab != tabs.end()) {
        tabs.erase(tab);
    }

    //Delete opened script
    auto scriptIter = VisualScriptManager::GetInstance().openedScripts.find(script->scriptId);
    if (scriptIter != VisualScriptManager::GetInstance().openedScripts.end()) {
        VisualScriptManager::GetInstance().openedScripts.erase(script->scriptId);
    }

    std::string str = "Deleted script \"";
    str += script->scriptId;
    str += "\"";
    Logger::Log("P", str.c_str());
	return true;
}

/*
PURPOSE: Saves the script
*/
bool VisualScriptManager::SaveScript(std::shared_ptr<VisualScript> script, std::string& projectDir)
{
    //Saves the script
    AssetSaver::SaveScriptToFile(script->ToJson(), projectDir, script->scriptId);

    return true;
}

/*
PURPOSE: Gets the instance of the class
*/
VisualScriptManager& VisualScriptManager::GetInstance()
{
	static VisualScriptManager manager;
	return manager;
}
