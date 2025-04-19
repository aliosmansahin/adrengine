#include "Engine.h"
#include "Window.h"

int main(void)
{
    //window
    if (!Window::GetInstance().CreateWindow(800, 600, "Adrengine"))
        return -1;

    //engine
    if (!Engine::GetInstance().InitEngine(Window::GetInstance().GetWindow()))
        return -1;

    /* main loop */
    while (!Window::GetInstance().ShouldClose())
    {
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