#include "pch.h"
#include "LayoutProfile.h"

#include "ServiceLocator.h"
#include "interfaces/IEngine/IEngine.h"

#include "MenuBar.h"

/*
PURPOSE: Adds a layout to the profiles map
*/
void LayoutProfile::AddLayout(const std::shared_ptr<WindowLayout>& layout)
{
	if (layout)
	{
		layouts[layout->name] = layout;
	}
}

/*
PURPOSE: Gets a layout from the profiles map by its name
*/
std::shared_ptr<WindowLayout> LayoutProfile::GetLayout(const std::string& name) const
{
	auto it = layouts.find(name);
	if (it != layouts.end())
	{
		return it->second;
	}
	return nullptr;
}

/*
PURPOSE: Applies the profile to all windows
*/
void LayoutProfile::ApplyProfileToWindow()
{
	for (const auto& [layoutName, layout] : layouts) {
		if (layoutName == "window_project_dialog") {
			WindowProjectDialog::GetInstance().ApplyLayout(layout);
		}
	}
}

/*
PURPOSE: Converts the profile to json
*/
nlohmann::json LayoutProfile::ToJson() const
{
	nlohmann::json j;
	j["profile-name"] = profileName;
	j["layouts"] = nlohmann::json::array();
	for (const auto& [name, layout] : layouts)
	{
		if (layout)
		{
			j["layouts"].push_back(layout->ToJson());
		}
	}
	return j;
}

/*
PURPOSE: Loads the profile from json
*/
void LayoutProfile::FromJson(const nlohmann::json& j)
{
	if (j.contains("profile-name")) profileName = j["profile-name"].get<std::string>();
	if (j.contains("layouts") && j["layouts"].is_array())
	{
		for (const auto& item : j["layouts"])
		{
			auto layout = std::make_shared<WindowLayout>();
			layout->FromJson(item);
			AddLayout(layout);
		}
	}
}

/*
PURPOSE: Creates a default layout profile
*/
void LayoutProfile::CreateDefault(const std::string& profileName) {
	this->profileName = profileName;

	std::shared_ptr<WindowLayout> windowProjectLayout = std::make_shared<WindowLayout>();

	WindowProjectDialog::GetInstance().SetWindowLayout(windowProjectLayout);
	WindowProjectDialog::GetInstance().SetDefaultLayout();
	
	layouts[windowProjectLayout->name] = windowProjectLayout;
}