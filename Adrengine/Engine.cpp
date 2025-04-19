#include "Engine.h"


Engine::~Engine()
{
    if (interfaceManager) delete interfaceManager;
    Graphics::GetInstance().ReleaseGraphics();
    EntityManager::GetInstance().ReleaseEntityManager();
    Logger::Log("P", "Cleared engine");
}

bool Engine::InitEngine(GLFWwindow* window)
{
    Logger::Log("P", "Starting engine");
    //set class variables
    this->window = window;

    //graphics engine
    if (!Graphics::GetInstance().InitGraphics(window))
        return false;

    //interface manager
    interfaceManager = new InterfaceManager();
    if (!interfaceManager->InitInterface(window))
        return false;

    //entity manager
    if (!EntityManager::GetInstance().InitEntityManager())
        return false;

    //simple entity
    std::string entityId = EntityManager::GetInstance().CreateEntity();

    //Logger::Log("P", entityId.c_str());

    //entityManager->RemoveEntity("0");

    return true;
}

void Engine::Update()
{
    EntityManager::GetInstance().UpdateEntities();
}

void Engine::Draw()
{
    /* Render here */
    Graphics::GetInstance().Clear();

    EntityManager::GetInstance().DrawEntities();

    interfaceManager->StartFrame();
    interfaceManager->DrawInterface();
    interfaceManager->EndFrame();
}
