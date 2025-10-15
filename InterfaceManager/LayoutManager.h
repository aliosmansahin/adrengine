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
	void UseProfile(const std::string& profileName);

	void CreateDefaultProfile(const std::string& profileName);
	void LoadProfileFromJson(const std::string& profileName, const nlohmann::json& json);
	void DeleteCurrentProfile();

	std::unordered_map<std::string, std::shared_ptr<LayoutProfile>>& GetProfiles() { return profiles; }
	bool IsProfileSelected(std::string profileName);
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
	//Stores all layouts by name
	std::unordered_map<std::string, std::shared_ptr<LayoutProfile>> profiles;

	//Current layout profile
	std::string currentProfile = "default";
	std::string defaultProfileName = "default";

	//Store the previous profile name
	std::string previousProfileName = "default";
};