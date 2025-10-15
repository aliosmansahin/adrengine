#include "pch.h"
#include "LayoutManager.h"

#include "MenuBar.h"

/*
PURPOSE: Saves the current layout to a file
*/
void LayoutManager::SaveLayout()
{
	//Save current profile to file
	if (!std::filesystem::create_directory("Layout")) {
		Logger::Log("I", "Couldn't create layout folder, might be created before");
	}

	std::ofstream currentProfileFile("Layout/current_profile.cfg");

	if (currentProfileFile.is_open()) {
		currentProfileFile << currentProfile;
		currentProfileFile.close();
	} else {
		Logger::Log("E", "Failed to open Layout/current_profile.cfg for saving current profile name");
	}

	//Save all profiles
	std::ofstream profilesFile("Layout/profiles.cfg");

	if (profilesFile.is_open()) {
		for (const auto& [profileName, profiles] : profiles) {
			profilesFile << profileName << std::endl;
		}

		profilesFile.close();
	}
	else {
		Logger::Log("E", "Failed to open Layout/profiles.cfg for saving profile names");
	}

	//Save each profile
	if (!std::filesystem::create_directory("Layout/Profiles")) {
		Logger::Log("I", "Couldn't create Layout/Profiles folder, might be created before");
	}

	for (const auto& [profileName, profile] : profiles) {
		std::ofstream profileFile("Layout/Profiles/" + profileName + ".cfg");

		if (profileFile.is_open()) {
			profileFile << profile->ToJson().dump();
			profileFile.close();
		}
		else {
			std::string errorStr = "Couldn't open Layout/Profiles/";
			errorStr += profileName;
			errorStr += ".cfg for saving profile file";

			Logger::Log("E", errorStr.c_str());
		}
	}

	Logger::Log("P", "Layout saved");
}

/*
PURPOSE: Loads a layout from a file
*/
void LayoutManager::LoadLayout()
{
	//Get current profile from file
	std::ifstream currentProfileFile("Layout/current_profile.cfg");
	if(currentProfileFile.is_open()) {
		std::getline(currentProfileFile, currentProfile);
		currentProfileFile.close();
	} else {
		Logger::Log("E", "Failed to open Layout/current_profile.cfg for loading current profile name");
	}

	// If the current profile is the default profile, use the default layout
	if (currentProfile == defaultProfileName) {
		UseDefaultLayout();
		return;
	}

	//Load each profile names line by line
	std::vector<std::string> profilesToBeLoaded;
	std::ifstream profilesFile("Layout/profiles.cfg");
	if (profilesFile.is_open()) {
		std::string profileName;
		while (std::getline(profilesFile, profileName)) {
			profilesToBeLoaded.push_back(profileName);
		}
		profilesFile.close();
	}
	else {
		Logger::Log("E", "Failed to open Layout/profiles.cfg for loading profile names");
	}

	//Load each profile from their files using their names
	for (const auto& profileName : profilesToBeLoaded) {
		std::ifstream profileFile("Layout/Profiles/" + profileName + ".cfg");
		
		if (profileFile.is_open()) {
			//Load each profile from their files
			nlohmann::json json;

			profileFile >> json;

			LoadProfileFromJson(profileName, json);

			profileFile.close();
		}
		else {
			std::string errorStr = "Couldn't open Layout/Profiles/";
			errorStr += profileName;
			errorStr += ".cfg for loading profile";

			Logger::Log("E", errorStr.c_str());
		}
	}

	UseProfile(currentProfile);
}

/*
PURPOSE: Sets layout parameters to default
*/
void LayoutManager::UseDefaultLayout()
{
	//Create a unique profile id
	std::string uniqueProfileId = CreateProfileId();

	CreateDefaultProfile(uniqueProfileId);
	currentProfile = uniqueProfileId;
}

/*
PURPOSE: Changes the current profile, then applies to all windows
*/
void LayoutManager::UseProfile(const std::string& profileName)
{
	this->currentProfile = profileName;

	//Find the profile from all profiles
	auto iter = profiles.find(profileName);
	if (iter == profiles.end())
		return;

	if (!iter->second)
		return;

	iter->second->ApplyProfileToWindow();
}

/*
PURPOSE: Creates a new layout profile
*/
void LayoutManager::CreateDefaultProfile(const std::string& profileName)
{
	std::shared_ptr<LayoutProfile> profile = std::make_shared<LayoutProfile>();

	profile->CreateDefault(profileName);

	profiles[profileName] = profile;
}

/*
PURPOSE: Loads a profile from its json content
*/
void LayoutManager::LoadProfileFromJson(const std::string& profileName, const nlohmann::json& json)
{
	std::shared_ptr<LayoutProfile> profile = std::make_shared<LayoutProfile>();

	profile->FromJson(json);

	profiles[profileName] = profile;
}

/*
PURPOSE: Deletes the current profile
*/
void LayoutManager::DeleteCurrentProfile()
{
	//Delete current profile
	auto iter = profiles.find(currentProfile);
	
	if (iter == profiles.end())
		return;

	profiles.erase(iter);

	//TODO: Delete the folder of this profile

	if (profiles.size() > 0) {
		//Get first profile from map to use it
		UseProfile(profiles.begin()->first);
	} else {
		//If there is no profile, create a new default profile
		UseDefaultLayout();
	}

	//TODO: Save new layout
}

/*
PURPOSE: Checks for if a profile that has the given name is selected
*/
bool LayoutManager::IsProfileSelected(std::string profileName)
{
	return profileName == currentProfile;
}

/*
PURPOSE: Returns a profile id which is not used by any profile
*/
std::string LayoutManager::CreateProfileId()
{
	int profileIndex = 0;
	std::string profileSuffix = "Profile ";
	std::string uniqueProfileId = "";

	while (true) {
		uniqueProfileId = profileSuffix + std::to_string(profileIndex);

		++profileIndex; //Increase profile id for next id to be checked

		auto iter = profiles.find(uniqueProfileId);

		//Check for profile id if it exists in all profiles
		if (iter != profiles.end())
			continue;

		break;

	}

	return uniqueProfileId;
}
