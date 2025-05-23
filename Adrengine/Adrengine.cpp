#include "Engine.h"
#include "Window.h"
#include "Localization.h"

int main(void)
{
    //initialize localization
    if (!Localization::LoadLanguage())
        return -1;

    //create window
    if (!Window::GetInstance().CreateWindow(1280, 720, "Adrengine"))
        return -1;

    //initialize engine 
    if (!Engine::GetInstance().InitEngine(Window::GetInstance().GetWindow()))
        return -1;

    //main loop, exits when the window closes
    while (!Window::GetInstance().ShouldClose())
    {
        //disable vsync
        glfwSwapInterval(0);

        /*
            FIXME:
            Update keys and mouse,
            this is supposed to be in the update function of engine
            but glfw is not working there
            when i fix this problem, i will move it there
        */
        InputManager::GetInstance().Update();

        /*
            FIXME:
            handles glfw request from other engines,
            as i mention above, when i fix the issue, i will remove it
        */
        Window::GetInstance().HandleGLFWRequests();

        //calling engine funcs
        Engine::GetInstance().Update();
        Engine::GetInstance().Draw();

        //window msgs and swapping buffers
        Window::GetInstance().SwapBuffers();
        Window::GetInstance().PollEvents();
    }

    //closing all engines and window
    Engine::GetInstance().CloseEngine();
    Window::GetInstance().CloseWindow();
    return 0;
}