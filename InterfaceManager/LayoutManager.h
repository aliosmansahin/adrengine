#pragma once

#include <fstream>
#include <filesystem>
#include <unordered_map>

#include "Logger.h"
#include "utils/Utils.h"

class LayoutManager
{
public:
	static LayoutManager& GetInstance() {
		static LayoutManager manager;
		return manager;
	}

public:
	void ChangeProfile(std::string profileId);
	void RenameCurrentProfile(std::string newName);
	void DeleteCurrentProfile();

	std::unordered_map<std::string, std::string>& GetProfilesList() { return profileList; }

	bool IsProfileSelected(std::string profileId) { return currentProfileId == profileId; }

	std::string CreateProfileId();
	std::string GetCurrentProfileFilePath();
	bool NeedDefaultLayout();
	void CreateDefaultLayout();

	void CreateProfile();

	void LoadData();
	void SaveData();

private:
  	std::string currentProfileId;
	std::unordered_map<std::string, std::string> profileList; //Hold ids and names ==> first -> id, second -> name
};