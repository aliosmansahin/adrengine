#pragma once

#include <string>
#include "nlohmann_json/json.hpp"

#include "imgui/imgui.h"

class WindowLayout
{
public:
	nlohmann::json ToJson() const
	{
		nlohmann::json j;
		j["name"] = name;
		j["is-open"] = isOpen;
		j["dock-id"] = dockId;
		j["dock-flags"] = dockFlags;
		j["window-flags"] = windowFlags;
		j["width"] = width;
		j["height"] = height;
		j["pos-x"] = posX;
		j["pos-y"] = posY;
		return j;
	}
	void FromJson(const nlohmann::json& j)
	{
		if (j.contains("name")) name = j["name"].get<std::string>();
		if (j.contains("is-open")) isOpen = j["is-open"].get<bool>();
		if (j.contains("dock-id")) dockId = j["dock-id"].get<ImGuiID>();
		if (j.contains("dock-flags")) dockFlags = j["dock-flags"].get<ImGuiDockNodeFlags>();
		if (j.contains("window-flags")) windowFlags = j["window-flags"].get<ImGuiWindowFlags>();
		if (j.contains("width")) width = j["width"].get<float>();
		if (j.contains("height")) height = j["height"].get<float>();
		if (j.contains("pos-x")) posX = j["pos-x"].get<float>();
		if (j.contains("pos-y")) posY = j["pos-y"].get<float>();
	}
public:
	std::string name; //Stores which window will use this windowlayout
	bool isOpen = true;
	ImGuiID dockId = 0;
	ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
	float width = 0.0f;
	float height = 0.0f;
	float posX = 0.0f;
	float posY = 0.0f;
};

class LayoutProfile
{
public:
	void AddLayout(const std::shared_ptr<WindowLayout>& layout);
	std::shared_ptr<WindowLayout> GetLayout(const std::string& name) const;

	void ApplyProfileToWindow();

	nlohmann::json ToJson() const;
	void FromJson(const nlohmann::json& j);

	void CreateDefault(const std::string& profileId);

public:
	void SetProfileName(const std::string& newProfileName) { profileName = newProfileName; }
	std::string& GetProfileName() { return profileName; }

private:
	std::string profileName;
	std::string profileId;
	std::unordered_map<std::string, std::shared_ptr<WindowLayout>> layouts;
};