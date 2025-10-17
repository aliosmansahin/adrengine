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
		Logger::Log("W", "Couldn't create layout folder, might be created before");
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
		Logger::Log("W", "Couldn't create Layout/Profiles folder, might be created before");
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
		Logger::Log("W", "Failed to open Layout/current_profile.cfg for loading current profile name");
	}

	// If the current profile is the default profile, use the default layout
	if (currentProfile == defaultProfileId) {
		Logger::Log("I", "Using default layout");
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

	//If there is no profile to be loaded, use the default layout
	if (profilesToBeLoaded.empty()) {
		Logger::Log("W", "No profiles to be loaded, using default layout");
		UseDefaultLayout();
		return;
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

			/*
				Create a default profile if the file couldn't be opened
				This is to prevent issues if the user deletes the profile file manually
			*/
			CreateDefaultProfile(profileName);
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
void LayoutManager::UseProfile(const std::string& profileId)
{
	//Find the profile from all profiles
	auto iter = profiles.find(profileId);
	if (iter == profiles.end())
		return;

	if (!iter->second)
		return;

	this->currentProfile = profileId;

	iter->second->ApplyProfileToWindow();
}

/*
PURPOSE: Creates a new layout profile
*/
void LayoutManager::CreateDefaultProfile(const std::string& profileId)
{
	std::shared_ptr<LayoutProfile> profile = std::make_shared<LayoutProfile>();

	profile->CreateDefault(profileId);

	profiles[profileId] = profile;
}

/*
PURPOSE: Loads a profile from its json content
*/
void LayoutManager::LoadProfileFromJson(const std::string& profileId, const nlohmann::json& json)
{
	std::shared_ptr<LayoutProfile> profile = std::make_shared<LayoutProfile>();

	profile->FromJson(json);

	profiles[profileId] = profile;
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

	//Delete the file of this profile
	std::filesystem::remove("Layout/Profiles/" + currentProfile + ".cfg");

	if (profiles.size() > 0) {
		//Get first profile from map to use it
		UseProfile(profiles.begin()->first);
	} else {
		//If there is no profile, create a new default profile
		UseDefaultLayout();
	}

	//Save new layout via saving all profiles
	SaveLayout();
}

/*
PURPOSE: Renames the current profile with given name
*/
void LayoutManager::RenameCurrentProfile(const std::string& newName)
{
	//Get current profile
	auto iter = profiles.find(currentProfile);

	if (iter == profiles.end())
		return;

	if (!iter->second)
		return;

	auto& profile = iter->second;

	//Set the profile name to the new
	iter->second->SetProfileName(newName);

	//Save layout to save new name
	SaveLayout();
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
	std::string profileSuffix = "Profile";
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
