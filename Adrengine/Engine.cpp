#include "Engine.h"


Engine::~Engine()
{
    if (interfaceManager) delete interfaceManager;
    Graphics::GetInstance().ReleaseGraphics();
    if (entityManager) delete entityManager;
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
    entityManager = new EntityManager();
    if (!entityManager->InitEntityManager())
        return false;

    //simple entity
    std::string entityId = entityManager->CreateEntity();

    //Logger::Log("P", entityId.c_str());

    //entityManager->RemoveEntity("0");

    return true;
}

void Engine::Update()
{
    entityManager->UpdateEntities();
}

void Engine::Draw()
{
    /* Render here */
    Graphics::GetInstance().Clear();

    entityManager->DrawEntities();

    interfaceManager->StartFrame();
    interfaceManager->DrawInterface();
    interfaceManager->EndFrame();
}
