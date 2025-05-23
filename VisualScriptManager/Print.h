#pragma once

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

#include <string>
#include <iostream>

#include "Node.h"

//Input for the node. Inherits from base node input
class PrintInput : public Input {
public:
	std::string text = "";
};

//Node class. Inherits from base node
class Print : public Node
{
public:
	//main functions
	VISUALSCRIPTMANAGER_API void				  Draw() override;
	VISUALSCRIPTMANAGER_API std::function<void()> Compile() override;
	VISUALSCRIPTMANAGER_API std::function<void()> Call(Input* input) override;

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPos(int x, int y) override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "Print";
	}
	VISUALSCRIPTMANAGER_API int					  GetIdPass() override {
		return 4;
	}
	VISUALSCRIPTMANAGER_API std::shared_ptr<Node> clone() override {
		return std::make_shared<Print>(*this);
	}
	VISUALSCRIPTMANAGER_API void				  SetId(int id) override {
		this->id = id;
	}
	VISUALSCRIPTMANAGER_API int					  GetId() override {
		return id;
	}

	//json
	VISUALSCRIPTMANAGER_API nlohmann::json		  ToJson() override;
	VISUALSCRIPTMANAGER_API bool				  FromJson(nlohmann::json json) override;
public:
	//input buffer for the text
	char buf[32] = "";
private:
	//variables
	int id = 0;
	int x, y;
	bool first = true;
};

