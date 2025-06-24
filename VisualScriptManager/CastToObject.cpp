#include "pch.h"
#include "CastToObject.h"

#include "interfaces/IEntity/IObject/IObject.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value CastToObject::Evaluate(Pin* pin)
{
    Value entityValue = EvaluateInput(inputPins[0].get());

    if (!std::holds_alternative<IEntity*>(entityValue)) {
        std::monostate{};
    }

    IEntity* entity = std::get<IEntity*>(entityValue);

    IObject* casted = dynamic_cast<IObject*>(entity);

    if (!casted)
        return std::monostate{};

    return casted;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void CastToObject::SetPins()
{
    inputPins = {
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Input, this),
    };

    outputPins = {
        std::make_shared<Pin>("Object", PinType::Object, PinDirection::Output, this),
    };
}
