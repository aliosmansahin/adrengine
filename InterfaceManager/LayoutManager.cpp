#include "pch.h"
#include "LayoutManager.h"

#include "ServiceLocator.h"
#include "interfaces/IProject/IProject.h"

#include "imgui/imgui_internal.h"

#include "MenuBar.h"

#include "InterfaceManager.h"

/*
	FILE STRUCTURE:
		-- Layout/
			-- Profiles/
				-- <profile_id>.ini
			-- profiles_list.cfg
			-- current_profile.cfg
*/

std::string layoutDirectory = "Layout/";
std::string profilesDirectory = layoutDirectory + "Profiles/";
std::string profilesListFile = layoutDirectory + "profiles_list.cfg";
std::string currentProfileFile = layoutDirectory + "current_profile.cfg";

/*
PURPOSE: Returns filepath of the current profile
*/
std::string LayoutManager::GetCurrentProfileFilePath()
{
	return std::string(profilesDirectory + currentProfileId + ".ini");
}

/*
PURPOSE: Returns true if a default layout must be created
*/
bool LayoutManager::NeedDefaultLayout()
{
	return !std::filesystem::exists(profilesDirectory + currentProfileId + ".ini");
}

/*
PURPOSE: Creates a layout for first profile openings
*/
void LayoutManager::CreateDefaultLayout()
{
	ImGuiID dockspaceId = ImHashStr("DockSpace");

	ImGuiID allScenes_DockId, gameViewport_AssetExplorer_Scene_EntityProperties_DockId;
	ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Right, 0.15f, &allScenes_DockId, &gameViewport_AssetExplorer_Scene_EntityProperties_DockId);

	ImGui::DockBuilderDockWindow(WindowAllScenes::GetInstance().GetWindowTitleWithID().c_str(), allScenes_DockId);

	ImGuiID gameViewport_AssetExplorer_DockId, scene_EntityProperties_DockId;
	ImGui::DockBuilderSplitNode(gameViewport_AssetExplorer_Scene_EntityProperties_DockId, ImGuiDir_Right, 0.75f, &gameViewport_AssetExplorer_DockId, &scene_EntityProperties_DockId);

	ImGuiID gameViewport_DockId, assetExplorer_DockId;
	ImGui::DockBuilderSplitNode(gameViewport_AssetExplorer_DockId, ImGuiDir_Up, 0.75f, &gameViewport_DockId, &assetExplorer_DockId);

	ImGuiID scene_DockId, entityProperties_DockId;
	ImGui::DockBuilderSplitNode(scene_EntityProperties_DockId, ImGuiDir_Up, 0.5f, &scene_DockId, &entityProperties_DockId);

	ImGui::DockBuilderDockWindow(WindowScene::GetInstance().GetWindowTitleWithID().c_str(), scene_DockId);
	ImGui::DockBuilderDockWindow(WindowEntityProperties::GetInstance().GetWindowTitleWithID().c_str(), entityProperties_DockId);
	ImGui::DockBuilderDockWindow(WindowGameViewport::GetInstance().GetWindowTitleWithID().c_str(), gameViewport_DockId);
	ImGui::DockBuilderDockWindow(WindowAssetExplorer::GetInstance().GetWindowTitleWithID().c_str(), assetExplorer_DockId);

	ImGui::DockBuilderDockWindow(WindowVisualScript::GetInstance().GetWindowTitleWithID().c_str(), entityProperties_DockId); //Dock into entityProperties_DockId

	ImGui::DockBuilderFinish(dockspaceId);
}

/*
PURPOSE: This function creates a pair and inserts it into profileList
	After restart the editor opens the new profile
*/
void LayoutManager::CreateProfile()
{
	currentProfileId = CreateProfileId();
	profileList.insert({ currentProfileId, currentProfileId });

	Logger::Log("P", std::string("LayoutManager::CreateProfile: Created default profile with ID: " + currentProfileId).c_str());
}

/*
PURPOSE: Changes current profile to given profile id
	Restart required after this
*/
void LayoutManager::ChangeProfile(std::string profileId)
{
	currentProfileId = profileId;
}

/*
PURPOSE: Deletes the current profile
	Restart required after this
*/
void LayoutManager::DeleteCurrentProfile()
{
	auto iter = profileList.find(currentProfileId);
	if (iter == profileList.end()) {
		Logger::Log("I", "LayoutManager::DeleteCurrentProfile: Current profile not found from the profiles list.");
		return;
	}

	profileList.erase(iter);

	std::filesystem::remove_all(profilesDirectory + currentProfileId + ".ini");

	// If there are still profiles left, set currentProfileId to the first profile
	if (!profileList.empty()) {
		currentProfileId = profileList.begin()->first; //Set to first profile
	}
	else {
		currentProfileId.clear(); // Clear currentProfileId if no profiles left
	}
}

/*
PURPOSE: Renames the current profile
*/
void LayoutManager::RenameCurrentProfile(std::string newName)
{
	auto profileIter = profileList.find(currentProfileId);
	if (profileIter == profileList.end()) {
		Logger::Log("E", std::string("LayoutManager::RenameCurrentProfile: Could not change current profile name: " + newName).c_str());
		return;
	}

	profileIter->second = newName;

	Logger::Log("P", std::string("LayoutManager::RenameCurrentProfile: Renamed current profile to: " + newName).c_str());
}

/*
PURPOSE: This function loads profileList and current profile id
Warning: Call this before ImGui initialization
*/
void LayoutManager::LoadData()
{
	/* Load all profile ids to load each of them */
	std::ifstream profilesFile(profilesListFile);

	if (!profilesFile.is_open()) {
		Logger::Log("I", std::string("LayoutManager::LoadData: Could not open profiles list file: " + profilesListFile).c_str());
	}
	else {
		std::string profileId;

		while (profilesFile >> profileId)
		{
			std::string profileName;

			std::getline(profilesFile, profileName);

			if (profileName.empty()) {
				profileList.insert({ profileId, profileId });
				break;
			}

			profileName.erase(profileName.begin());

			profileList.insert({ profileId, profileName });
		}

		profilesFile.close();
	}

	/* Load current profile id */
	std::ifstream currentFile(currentProfileFile);
	if (!currentFile.is_open()) {
		Logger::Log("I", std::string("LayoutManager::LoadData: Could not open current profile file: " + currentProfileFile).c_str());

		/* Set first profile as current profile */
		if (!profileList.empty())
			currentProfileId = profileList.begin()->first;
		else
			CreateProfile();
	}
	else {
		currentFile >> currentProfileId;

		currentFile.close();
	}

	Logger::Log("P", "Loaded layout data");
}

/*
PURPOSE: This function saves profileList and current profile id
*/
void LayoutManager::SaveData()
{
	/* Create necessary folders */
	if (!std::filesystem::exists(layoutDirectory)) {
		std::filesystem::create_directories(layoutDirectory);
	}

	if (!std::filesystem::exists(profilesDirectory)) {
		std::filesystem::create_directories(profilesDirectory);
	}

	/* Save profilesList */
	std::ofstream profilesFile(profilesListFile);

	for (const auto& [id, name] : profileList) {
		profilesFile << id << " " << name << std::endl;
	}

	profilesFile.close();

	/* Save current profile */
	std::ofstream currentFile(currentProfileFile);

	currentFile << currentProfileId;

	currentFile.close();
	
	Logger::Log("P", "Saved layout data");
}

/*
PURPOSE: Creates a unique profile id
*/
std::string LayoutManager::CreateProfileId()
{
	//Generate a unique profile id via checking existing ids
	int index = 0;
	std::string profileId;
	while (true) {
		profileId = "profile" + std::to_string(index);
		if (profileList.find(profileId) == profileList.end())
			break;
		++index;
	}

	return profileId;
}
