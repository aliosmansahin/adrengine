#include "pch.h"
#include "InputManager.h"

//Implementations for static variables
std::unordered_map<int, bool> InputManager::currentKeys;
std::unordered_map<int, bool> InputManager::previousKeys;
std::unordered_map<int, bool> InputManager::currentMouseButtons;
std::unordered_map<int, bool> InputManager::previousMouseButtons;
bool InputManager::mouseVisibility = true;
int InputManager::mouseX = 0;
int InputManager::mouseY = 0;

/*
PURPOSE: Initialize input engine
*/
bool InputManager::InitEngine()
{

    //glfwSetMouseButtonCallback(window, MouseButtonCallback);
    return true;
}

/*
PURPOSE: Release input engine
*/
void InputManager::ReleaseEngine()
{
    //glfwSetMouseButtonCallback(window, nullptr);
    Logger::Log("P", "Closed input manager");
}

/*
PURPOSE: Returns true when the key is pressed in first frame.
    After first frame, it returns false even the key is pressed
*/
bool InputManager::IsKeyJustPressed(int key)
{
    return currentKeys[key] && !previousKeys[key]; // prev is false, current is true
}

/*
PURPOSE: Returns true when the key is released in first frame.
    After first frame, it returns false even the key is released
*/
bool InputManager::IsKeyJustReleased(int key)
{
    return !currentKeys[key] && previousKeys[key]; // prev is true, current is false
}

/*
PURPOSE: Returns true when the key is pressed
*/
bool InputManager::IsKeyPressed(int key)
{
    return currentKeys[key];
}

/*
PURPOSE: Returns true when the mouse button is pressed in first frame.
    After first frame, it returns false even the mouse button is pressed
*/
bool InputManager::IsMouseButtonJustPressed(int key)
{
    return currentMouseButtons[key] && !previousMouseButtons[key];
}

/*
PURPOSE: Returns true when the mouse button is released in first frame.
    After first frame, it returns false even the mouse button is released
*/
bool InputManager::IsMouseButtonJustReleased(int key)
{
    return !currentMouseButtons[key] && previousMouseButtons[key];
}

/*
PURPOSE: Returns true when the mouse button is pressed
*/
bool InputManager::IsMouseButtonPressed(int key)
{
    return currentMouseButtons[key];
}

/*
PURPOSE: Returns the mouse visibility
*/
INPUTMANAGER_API bool InputManager::GetMouseVisibility()
{
    return mouseVisibility;
}

/*
PURPOSE: Requests to window class to set mouse visibility
*/
void InputManager::RequestSetMouseVisibility(bool visible)
{
    /*
        Send to the window class to execute and handle answer,
        change request variable to true
    */

    mouseVisibility = visible;
    requestSetMouseVisibility = true;
    /* if (visible) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }*/
}

/*
PURPOSE: Returns true if there is a request to set mouse visibility,
    after that, disable the request
*/
INPUTMANAGER_API bool InputManager::GetRequestSetMouseVisibility()
{
    bool tmp = requestSetMouseVisibility;
    requestSetMouseVisibility = false;
    return tmp;
}

/*
PURPOSE: Requests to window class to get status of mouse buttons
*/
INPUTMANAGER_API void InputManager::RequestGetMouseButtons()
{
    requestGetMouseButtons = true;
}

/*
PURPOSE: This function will be called in window class to apply mouse buttons in input manager
*/
INPUTMANAGER_API void InputManager::AnswerGetMouseButtons(std::unordered_map<int, bool>& currentMouseButtons)
{
    this->currentMouseButtons = currentMouseButtons;
}

/*
PURPOSE: Returns true if there is a request to get status of mouse buttons,
    after that, disable the request
*/
INPUTMANAGER_API bool InputManager::GetRequestGetMouseButtons()
{
    bool tmp = requestGetMouseButtons;
    requestGetMouseButtons = false;
    return tmp;
}

/*
PURPOSE: Requests to window class to get mouse position
*/
INPUTMANAGER_API void InputManager::RequestGetMousePos()
{
    requestGetMousePos = true;
}

/*
PURPOSE: This function will be called in window class to apply mouse position in input manager
*/
INPUTMANAGER_API void InputManager::AnswerGetMousePos(int mx, int my)
{
    mouseX = mx;
    mouseY = my;
}

/*
PURPOSE: Returns true if there is a request to get mouse positions,
    after that, disable the request
*/
INPUTMANAGER_API bool InputManager::GetRequestGetMousePos()
{
    bool tmp = requestGetMousePos;
    requestGetMousePos = false;
    return tmp;
}

/*
PURPOSE: Requests to window class to get status of keys
*/
INPUTMANAGER_API void InputManager::RequestGetKeys()
{
    requestGetKeys = true;
}

/*
PURPOSE: This function will be called in window class to apply keys in input manager
*/
INPUTMANAGER_API void InputManager::AnswerGetKeys(std::unordered_map<int, bool>& currentKeys)
{
    this->currentKeys = currentKeys;
}

/*
PURPOSE: Returns true if there is a request to get status of keys,
    after that, disable the request
*/
INPUTMANAGER_API bool InputManager::GetRequestGetKeys()
{
    return requestGetKeys;
}

/*
PURPOSE: Returns mouse position X
*/
int InputManager::GetMouseX()
{
    return mouseX;
}

/*
PURPOSE: Returns mouse position Y
*/
int InputManager::GetMouseY()
{
    return mouseY;
}

/*
PURPOSE: Sets mouse position to given parameters
*/
void InputManager::SetMousePos(int x, int y)
{
    //glfwSetCursorPos(window, x, y);
}

/*
PURPOSE: Updates the input manager
*/
void InputManager::Update()
{
    //Set previous keys to current keys before changing currents
    previousKeys = currentKeys;
    previousMouseButtons = currentMouseButtons;

    //Request all buttons, keys...
    RequestGetMouseButtons();

    RequestGetMousePos();

    RequestGetKeys();
}

/*
PURPOSE: Gets the instance of the class
*/
InputManager& InputManager::GetInstance()
{
    static InputManager engine;
    return engine;
}
