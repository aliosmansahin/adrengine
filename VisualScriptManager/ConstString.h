#pragma once

#include "Node.h"

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

class ConstString : public Node
{
public:
	//drawing
	VISUALSCRIPTMANAGER_API void				  Draw(std::shared_ptr<NodeVisual> nodeVisual) override;

	//execution
	VISUALSCRIPTMANAGER_API Value				  Evaluate(std::shared_ptr<IPin> pin) override;

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPins() override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "ConstString";
	}

	//clone
	VISUALSCRIPTMANAGER_API std::shared_ptr<INode> clone() override {
		return std::make_shared<ConstString>(*this);
	}

	//json
	VISUALSCRIPTMANAGER_API nlohmann::json		  ToJson() override;
	VISUALSCRIPTMANAGER_API bool				  FromJson(nlohmann::json json) override;
private:
	char buf[32]{0};
};
