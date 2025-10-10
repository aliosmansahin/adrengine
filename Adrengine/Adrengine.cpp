#include "Engine.h"
#include "Window.h"
#include "Localization.h"

#include "ServiceLocator.h"

int main(void)
{
    //initialize localization
    if (!Localization::LoadLanguage())
        return -1;

    //create window
    if (!Window::GetInstance().CreateWindow(1280, 720, "Adrengine"))
        return -1;

    //initialize engine
	ServiceLocator::Register<IEngine>(&Engine::GetInstance());
    if (!ServiceLocator::Get<IEngine>()->InitEngine(Window::GetInstance().GetWindow()))
        return -1;

    //main loop, exits when the window closes
    while (!Window::GetInstance().ShouldClose())
    {
        //disable vsync
        glfwSwapInterval(0);

        //calling engine funcs
        ServiceLocator::Get<IEngine>()->Update();
        ServiceLocator::Get<IEngine>()->Draw();

        //window msgs and swapping buffers
        Window::GetInstance().SwapBuffers();
        Window::GetInstance().PollEvents();
    }

    //closing all engines and window
    ServiceLocator::Get<IEngine>()->CloseEngine();
    Window::GetInstance().CloseWindow();
    return 0;
}