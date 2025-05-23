#pragma once

/*
	This node has no input and output
*/

#include "Node.h"

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

class Begin : public Node
{
public:
	//main functions
	VISUALSCRIPTMANAGER_API void				  Draw() override;
	VISUALSCRIPTMANAGER_API std::function<void()> Compile() override;
	VISUALSCRIPTMANAGER_API std::function<void()> Call(Input* input) override;

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPos(int x, int y) override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "Begin";
	}
	VISUALSCRIPTMANAGER_API int					  GetIdPass() override {
		return 0;
	}
	VISUALSCRIPTMANAGER_API std::shared_ptr<Node> clone() override {
		return std::make_shared<Begin>(*this);
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
private:
	//variables
	int id = 0;
	int x = 0, y = 0;
	bool first = true;
};

