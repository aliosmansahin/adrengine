#include "pch.h"
#include "GetThisEntity.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value GetThisEntity::Evaluate(Pin* pin)
{
    return entity;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void GetThisEntity::SetPins()
{
    outputPins = {
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Output, this),
    };
}