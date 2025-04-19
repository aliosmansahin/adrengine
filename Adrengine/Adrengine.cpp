#include "Engine.h"
#include "Window.h"

int main(void)
{
    //window
    Window* window = new Window();
    if (!window->CreateWindow(800, 600, "Adrengine"))
        return -1;

    //engine
    if (!Engine::GetInstance().InitEngine(window->GetWindow()))
        return -1;

    /* main loop */
    while (!window->ShouldClose())
    {
        //calling engine funcs
        Engine::GetInstance().Update();
        Engine::GetInstance().Draw();

        //window msgs and swapping buffers
        window->SwapBuffers();
        window->PollEvents();
    }

    Engine::GetInstance().CloseEngine();
    delete window;
    return 0;
}