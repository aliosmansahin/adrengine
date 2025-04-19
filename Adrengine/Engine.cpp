#include "Engine.h"


Engine::~Engine()
{
    if (interfaceManager) delete interfaceManager;
    if (graphics) delete graphics;
    if (entityManager) delete entityManager;
    Logger::Log("P", "Cleared engine");
}

bool Engine::InitEngine(GLFWwindow* window)
{
    Logger::Log("P", "Starting engine");
    //set class variables
    this->window = window;

    //graphics engine
    graphics = new Graphics();
    if (!graphics->InitGraphics(window))
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
    graphics->Clear();

    entityManager->DrawEntities();

    interfaceManager->StartFrame();
    interfaceManager->DrawInterface();
    interfaceManager->EndFrame();
}
