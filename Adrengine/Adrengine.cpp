#include "Engine.h"
#include "Window.h"

int main(void)
{
    //window
    Window* window = new Window();
    if (!window->CreateWindow(800, 600, "Adrengine"))
        return -1;

    //engine
    Engine* engine = new Engine();
    if (!engine->InitEngine(window->GetWindow()))
        return -1;

    /* main loop */
    while (!window->ShouldClose())
    {
        //calling engine funcs
        engine->Update();
        engine->Draw();

        //window msgs and swapping buffers
        window->SwapBuffers();
        window->PollEvents();
    }

    delete engine;
    delete window;
    return 0;
}