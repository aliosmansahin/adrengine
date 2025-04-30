#pragma once

#include <fstream>
#include <filesystem>

#include "nlohmann_json/json.hpp"

#include "Scene.h"
#include "Engine.h"

class Scene;

class FileIO
{
public:
	
private:
	FileIO() = default;
	~FileIO() = default;
	FileIO(const FileIO&) = delete;
	FileIO& operator=(const FileIO&) = delete;
};