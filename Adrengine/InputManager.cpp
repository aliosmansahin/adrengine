#include "InputManager.h"

std::unordered_map<int, bool> InputManager::currentKeys;
std::unordered_map<int, bool> InputManager::previousKeys;
std::unordered_map<int, bool> InputManager::currentMouseButtons;
std::unordered_map<int, bool> InputManager::previousMouseButtons;
bool InputManager::mouseVisibility = true;
int InputManager::mouseX = 0.0;
int InputManager::mouseY = 0.0;

bool InputManager::InitEngine(GLFWwindow* window)
{
    this->window = window;
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    //glfwSetMouseButtonCallback(window, MouseButtonCallback);
    return true;
}

void InputManager::ReleaseEngine()
{
    glfwSetKeyCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
    //glfwSetMouseButtonCallback(window, nullptr);
}

bool InputManager::IsKeyJustPressed(int key)
{
    return currentKeys[key] && !previousKeys[key]; // prev is false, current is true
}

bool InputManager::IsKeyJustReleased(int key)
{
    return !currentKeys[key] && previousKeys[key]; // prev is true, current is false
}

bool InputManager::IsKeyPressed(int key)
{
    return currentKeys[key];
}

bool InputManager::IsMouseButtonJustPressed(int key)
{
    return currentMouseButtons[key] && !previousMouseButtons[key];
}

bool InputManager::IsMouseButtonJustReleased(int key)
{
    return !currentMouseButtons[key] && previousMouseButtons[key];
}

bool InputManager::IsMouseButtonPressed(int key)
{
    return currentMouseButtons[key];
}

void InputManager::SetMouseVisibility(bool visible)
{
    mouseVisibility = visible;
    if (visible)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

int InputManager::GetMouseX()
{
    return mouseX;
}

int InputManager::GetMouseY()
{
    return mouseY;
}

void InputManager::SetMousePos(int x, int y)
{
    glfwSetCursorPos(window, x, y);
}

void InputManager::Update()
{
    previousKeys = currentKeys;
    previousMouseButtons = currentMouseButtons;

    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
        currentMouseButtons[i] = glfwGetMouseButton(window, i);
    }

    // callback functions was out
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(mouseVisibility)
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (action == GLFW_PRESS)
        currentKeys[key] = true;
    else if (action == GLFW_RELEASE)
        currentKeys[key] = false;
}

void InputManager::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if(mouseVisibility)
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    mouseX = xpos;
    mouseY = ypos;
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(mouseVisibility)
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    if (action == GLFW_PRESS)
        currentMouseButtons[button] = true;
    else if(action == GLFW_RELEASE)
        currentMouseButtons[button] = false;
}

InputManager& InputManager::GetInstance()
{
    static InputManager engine;
    return engine;
}
