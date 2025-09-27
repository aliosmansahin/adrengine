#include "pch.h"
#include "Localization.h"

//Statics
std::string Localization::current = "english";
std::unordered_map<std::string, std::string> Localization::language;

/*
PURPOSE: Initialize current language
	Loads language by key-value
	Keys will be used by get value text depends on current language

	This function have to be called before all using texts
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
		Logger::Log("E", "Current language file couldn't be loaded, english will be loaded");
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
		//Remove the first character which is a space
		std::getline(file, value);
		value.erase(value.begin());
		language.insert({ key, value });
	}

	//close file
	file.close();
	return true;
}

/*
PURPOSE: Gets value text in active language from a key
	if the key doesn't exist in localization, returns the key
*/
const char* Localization::GetString(std::string key)
{
	auto item = language.find(key);
	if (item == language.end()) {
		//GetString will return key if there is not a value for this key
		const char* keyStr = key.c_str();
		return keyStr;
	}
	return item->second.c_str();
}
