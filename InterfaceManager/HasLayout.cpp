#include "pch.h"
#include "HasLayout.h"

/*
PURPOSE: When the first frame value is true, it sets the next window size and position to the values stored in windowLayout
	this is used to set the initial size and position of the window when it is created or when the layout is changed
*/
void HasLayout::UpdateFirstFrame()
{
    //Set the dock id
    ImGui::SetNextWindowDockID(windowLayout->dockId);

    //Set initial size and position
    if (firstFrame) {
        firstFrame = false;
        ImGui::SetNextWindowSize(ImVec2(windowLayout->width, windowLayout->height));
        ImGui::SetNextWindowPos(ImVec2(windowLayout->posX, windowLayout->posY));
    }
}

/*
PURPOSE: Changes layout of this window, and changes firstFrame variable to true to apply parameters of imgui window
*/
void HasLayout::ApplyLayout(std::shared_ptr<WindowLayout> newWindowLayout)
{
    windowLayout = newWindowLayout;
    firstFrame = true;
}
