#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

#include "LayoutProfile.h"

class LayoutManager
{
public:
	//Get singleton instance
	static LayoutManager& GetInstance()
	{
		static LayoutManager instance;
		return instance;
	}

public:
	//Handle layout
	void SaveLayout();
	void LoadLayout();
	void UseDefaultLayout();
	void UseProfile(const std::string& profileId);

	void CreateDefaultProfile(const std::string& profileId);
	void LoadProfileFromJson(const std::string& profileId, const nlohmann::json& json);
	void DeleteCurrentProfile();
	void RenameCurrentProfile(const std::string& newName);

	std::unordered_map<std::string, std::shared_ptr<LayoutProfile>>& GetProfiles() { return profiles; }
	bool IsProfileSelected(std::string profileId);
	std::string CreateProfileId();

private:
	//Singleton pattern
	LayoutManager() = default;
	~LayoutManager() = default;
	LayoutManager(const LayoutManager&) = delete;
	LayoutManager& operator=(const LayoutManager&) = delete;
	LayoutManager(LayoutManager&&) = delete;
	LayoutManager& operator=(LayoutManager&&) = delete;

private:
	//Stores all layouts by name : first -> id, second -> the object of the profile
	std::unordered_map<std::string, std::shared_ptr<LayoutProfile>> profiles;

	//Current layout profile id
	std::string currentProfile = "default";
	std::string defaultProfileId = "default";

	//Store the previous profile id
	std::string previousProfileId = "default";
};