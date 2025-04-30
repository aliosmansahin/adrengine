#include "Localization.h"

bool Localization::LoadLanguage()
{
	Logger::Log("P", "Loading language");

	//open current language file and check if it opened
	std::ifstream currentFile("localization/current");
	if (currentFile.is_open()) {
		currentFile >> current;
		currentFile.close();
	}
	else {
		Logger::Log("E", "Current language file couldn't be loaded, english will have loaded");
		current = "english";
	}

	//clear the language map
	language.clear();

	//open language file and check if it is opened
	std::ifstream file("localization/" + current);
	if (!file.is_open()) {
		Logger::Log("E", "Language file couldn't be loaded");
		return false;
	}

	//load each key and value and insert it to the unordered_map
	std::string key;
	std::string value;

	while (file >> key) {
		std::getline(file, value);
		value.erase(value.begin());
		language.insert(std::pair<const char*, const char*>(key.c_str(), value.c_str()));
	}

	//close file
	file.close();
	return true;
}

const char* Localization::GetString(std::string key)
{
	auto item = language.find(key);
	if (item == language.end())
		return "";
	return item->second.c_str();
}
