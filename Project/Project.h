#pragma once

#ifdef PROJECT_EXPORTS
#define PROJECT_API __declspec(dllexport)
#else
#define PROJECT_API __declspec(dllimport)
#endif

#include <string>
#include <fstream>

#include <Logger.h>
#include <AssetDatabase.h>
#include <SceneManager.h>
#include <InterfaceManager.h>

#include <nlohmann_json/json.hpp>

class Project
{
private:
	//Singleton
	Project() = default;
	~Project() = default;
	Project(const Project&) = delete;
	Project& operator=(const Project&) = delete;

public:
	//getter for the instance
	PROJECT_API static Project& Get();

public:
	PROJECT_API bool CreateProject(std::string projectPath, std::string projectName);
	PROJECT_API bool OpenProject(std::string& projectName, std::string& projectFolder,
		std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes);
	PROJECT_API bool CloseProject();
	PROJECT_API bool SaveProject();
	PROJECT_API std::string& GetProjectFileLocation();
	PROJECT_API std::string& GetProjectDir();
private:
	//project specifications
	std::string projectName = "";
	std::string projectPath = "";
	std::string projectDir = "";
	std::string projectFileLocation = "";
};

