#pragma once

#include "Node.h"

#include <sstream>

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif


class ToString : public Node
{
public:
	//execution
	VISUALSCRIPTMANAGER_API virtual Value		  Evaluate(Pin* pin);

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPins() override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "ToString";
	}

	//clone
	VISUALSCRIPTMANAGER_API std::shared_ptr<Node> clone() override {
		return std::make_shared<ToString>(*this);
	}
private:
	//convert
	template<typename T>
	std::string ArithmeticToString(T value);
};

/*    CONVERT FUNCTIONS    */
template<typename T>
inline std::string ToString::ArithmeticToString(T value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}
