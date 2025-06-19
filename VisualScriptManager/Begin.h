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
	//drawing
	VISUALSCRIPTMANAGER_API void				  Draw(NodeVisual* nodeVisual) override;

	//execution
	VISUALSCRIPTMANAGER_API void				  Execute() override;

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPins() override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "Begin";
	}

	//clone
	VISUALSCRIPTMANAGER_API std::shared_ptr<Node> clone() override {
		return std::make_shared<Begin>(*this);
	}
};

