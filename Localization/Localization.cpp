#include "pch.h"
#include "Localization.h"

std::string Localization::current = "english";
std::unordered_map<std::string, std::string> Localization::language;

/*
PURPOSE: Initialize current language
*/
bool Localization::LoadLanguage()
{
	//some log
	Logger::Log("P", "Loading language");

	//open current language file and check if it opened
	std::ifstream currentFile("localization/current");
	if (currentFile.is_open()) {
		//get the current language and close the file
		currentFile >> current;
		currentFile.close();
	}
	else {
		/*
			we are not closing when program can't find out which language will be used,
			we will use english for default
		*/
		Logger::Log("E", "Current language file couldn't be loaded, english will have loaded");
		current = "english";
	}

	//clear the language map
	language.clear();

	//open language file and check if it is opened
	std::ifstream file("localization/" + current);
	if (!file.is_open()) {
		// if we can't open the file, we will exit
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

/*
PURPOSE: To get value of key that was requested
*/
const char* Localization::GetString(std::string key)
{
	auto item = language.find(key);
	if (item == language.end())
		return "";
	return item->second.c_str();
}
