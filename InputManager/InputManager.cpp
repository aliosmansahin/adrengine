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
bool InputManager::InitEngine(GLFWwindow* window, ImGuiContext* context)
{
    //Set glfw window pointer
    this->window = window;

    //Set imgui context
    ImGui::SetCurrentContext(context);

    //Set glfw callbacks
    glfwSetCursorPosCallback(window, InputManager::CursorPosCallback);
    
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
PURPOSE: Sets the visibility of the mouse
*/
INPUTMANAGER_API void InputManager::SetMouseVisibility(bool visibility)
{
    mouseVisibility = visibility;
    if (visibility) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
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
    glfwSetCursorPos(window, x, y);
}

/*
PURPOSE: Callback for updating mouse position
*/
INPUTMANAGER_API void InputManager::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    //To prevent imgui hover effects when mouse is disabled
    if(mouseVisibility)
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    //Update mouse position
    mouseX = (int)xpos;
    mouseY = (int)ypos;
}

/*
PURPOSE: Updates the input manager
*/
void InputManager::Update()
{
    //Set previous keys to current keys before changing currents
    previousKeys = currentKeys;
    previousMouseButtons = currentMouseButtons;

    //Update keys
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        int state = glfwGetKey(window, key);
        currentKeys[key] = (state == GLFW_PRESS || state == GLFW_REPEAT);
    }

    //Update mouse buttons
    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
        currentMouseButtons[i] = glfwGetMouseButton(window, i);
    }
}

/*
PURPOSE: Gets the instance of the class
*/
InputManager& InputManager::GetInstance()
{
    static InputManager engine;
    return engine;
}
