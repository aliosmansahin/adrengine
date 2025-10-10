#pragma once

#include <string>
#include <unordered_map>

#include "imgui/imgui.h"

struct WindowLayout
{
	std::string name;
	bool isOpen = true;
	ImGuiID dockId = 0;
	ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
	float widthRatio = 0.2f; // Ratio of the window width to the total width
	float heightRatio = 0.2f; // Ratio of the window height to the total height
};

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
	std::unordered_map<std::string, WindowLayout> layouts;
};