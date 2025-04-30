#include "Engine.h"
#include "Window.h"
#include "Localization.h"

int main(void)
{
    //localization
    if (!Localization::Get().LoadLanguage())
        return -1;

    //window
    if (!Window::GetInstance().CreateWindow(1280, 720, "Adrengine"))
        return -1;

    //engine
    if (!Engine::GetInstance().InitEngine(Window::GetInstance().GetWindow()))
        return -1;

    /* main loop */
    while (!Window::GetInstance().ShouldClose())
    {

        //glfwSwapInterval(1);
        //calling engine funcs
        Engine::GetInstance().Update();
        Engine::GetInstance().Draw();

        //window msgs and swapping buffers
        Window::GetInstance().SwapBuffers();
        Window::GetInstance().PollEvents();
    }


    Engine::GetInstance().CloseEngine();
    Window::GetInstance().CloseWindow();
    return 0;
}