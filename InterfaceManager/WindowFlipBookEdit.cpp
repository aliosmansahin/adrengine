#include "pch.h"
#include "WindowFlipBookEdit.h"

/*
PURPOSE: Draws the window
*/
INTERFACEMANAGER_API void WindowFlipBookEdit::DrawWindow()
{
    if (!edittingFlipBook) {
        showWindow = false;
        return;
    }
    std::shared_ptr<IFlipBookParams> params = std::dynamic_pointer_cast<IFlipBookParams>(edittingFlipBook->GetEntityParams());
    if (!params) {
        showWindow = false;
        return;
    }

    ImGui::Begin("Edit Flip Book", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);


    //Store entity id
    std::string textureIdStr = params->GetTextureId();

    static char textureIdBuf[32] = "";

    if (textureIdBuf != textureIdStr) { // if the entity id changed
        strncpy_s(textureIdBuf, textureIdStr.c_str(), sizeof(textureIdBuf));
        textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
    }

    //Get texture Id
    if (ImGui::InputText("Texture Id", textureIdBuf, sizeof(textureIdBuf), ImGuiInputTextFlags_EnterReturnsTrue)) { //CHANGE IT TO OPTION PANEL THAT CONTAINS ALL TEXTURES
        if (textureIdBuf[0] == '\0') {// if the input is empty
            strncpy_s(textureIdBuf, params->GetName().c_str(), sizeof(textureIdBuf));
            textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
        }
        else {
            if (AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf)).get()) {
                params->SetTextureId(std::string(textureIdBuf));
                params->SetTexture(AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf))->texture);
            }
        }
    }

    //Drag drop for texture id
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_ID")) {
            const char* payloadData = static_cast<const char*>(payload->Data);
            std::string droppedId(payloadData, payload->DataSize);

            auto& textures = AssetDatabase::GetInstance().GetTextures();
            auto textureIter = textures.find(droppedId);

            if (textureIter != textures.end()) {
                auto& texture = textureIter->second;
                if (texture.get()) {
                    params->SetTextureId(texture->id);
                    params->SetTexture(texture->texture);
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    //Draw the inspector texture for tile map
    if (!params->GetTextureId().empty()) {
        auto& textures = AssetDatabase::GetInstance().GetTextures();
        auto textureIter = textures.find(params->GetTextureId());
        int textureWidth = textureIter->second->width;
        int textureHeight = textureIter->second->height;

        static int frameWidth = 48;
        static int frameHeight = 48;

        //Warning text
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Creating inspector will delete old frames!");

        //Create framebuffer for inspector
        if (ImGui::Button("Create Inspector")) {
            edittingFlipBook->CreateInspectFrameBuffer((float)textureWidth, (float)textureHeight, frameWidth, frameHeight);
        }

        if (textureIter != textures.end() && edittingFlipBook->IsInspectCreated()) {
            //Update and render the inspector to the framebuffer
            edittingFlipBook->UpdateInspect();
            edittingFlipBook->DrawInspect(textureWidth, textureHeight, frameWidth, frameHeight);

            //Draw the inspector
            ImGui::SeparatorText("Inspector");
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 windowPos = ImGui::GetCursorPos();
            ImGui::GetWindowDrawList()->AddImage(
                (ImTextureID)(intptr_t)edittingFlipBook->GetInspectTexture(),
                ImVec2(pos.x, pos.y),
                ImVec2(pos.x + textureWidth, pos.y + textureHeight),
                ImVec2(0, 0),
                ImVec2(1, 1)
            );

            //Some visuals
            int padding = 10;
            ImGui::SetCursorPos(ImVec2(windowPos.x, windowPos.y + textureHeight + padding));

            //Write parameters
            ImGui::Text(std::string("Texture Size: " + std::to_string(textureWidth) + "x" + std::to_string(textureHeight) + "px").c_str());
            ImGui::Text(std::string("Tile Size: " + std::to_string(frameWidth) + "x" + std::to_string(frameHeight) + "px").c_str());

            //Set the size of each tiles
            ImGui::DragInt("Frame Width", &frameWidth);
            ImGui::DragInt("Frame Height", &frameHeight);

            if (ImGui::Button("Create Frames")) {
                //Create each tile
                edittingFlipBook->CreateFrames((float)textureWidth, (float)textureHeight, (float)frameWidth, (float)frameHeight);
            }

            auto& createdFrames = edittingFlipBook->GetCreatedFrames();
            if (!createdFrames.empty()) {
                ImGui::SeparatorText("Created Tiles");
                ImVec2 windowPos = ImGui::GetCursorPos();
                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImVec2 startPos = ImGui::GetCursorScreenPos();

                for (int i = 0; i < createdFrames.size(); ++i) {
                    std::shared_ptr<IFlipBookFrame> frame = createdFrames[i].second;

                    pos = ImGui::GetCursorScreenPos();

                    //Pass the coordinates
                    int tileX = frame->GetXY().first;
                    int tileY = frame->GetXY().second;
                    int currentTileWidth = frame->GetSize().first;
                    int currentTileHeight = frame->GetSize().second;
                    float u = frame->GetUV().first;
                    float v = frame->GetUV().second;
                    float tw = frame->GetTextureSize().first;
                    float th = frame->GetTextureSize().second;

                    //Set the position of the preview of the tile
                    pos.x += tileX * (frameWidth + padding);
                    pos.y += tileY * (frameHeight + padding);

                    //Image Button spec
                    ImVec2 imageSize = ImVec2((float)edittingFlipBook->GetFrameSize().first, (float)edittingFlipBook->GetFrameSize().second);
                    std::string imageButtonId = "##image_button" + std::to_string(i);

                    //Calculate current button positions
                    ImVec2 currentButtonPos;
                    currentButtonPos.x = windowPos.x + tileX * (edittingFlipBook->GetFrameSize().first + padding);
                    currentButtonPos.y = windowPos.y + tileY * (edittingFlipBook->GetFrameSize().second + padding);

                    //Change color when its selected
                    bool selected = createdFrames[i].first;
                    ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.5, 1.0f);
                    if (selected)
                        bgColor = ImVec4(0.0f, 0.7f, 0.0, 1.0f);

                    //Set the position of the button
                    ImGui::SetCursorPos(currentButtonPos);

                    //Some color sets
                    ImGui::PushStyleColor(ImGuiCol_Button, bgColor);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

                    //Draw the tile
                    if (ImGui::ImageButton(
                        imageButtonId.c_str(),
                        (ImTextureID)(intptr_t)params->GetTexture(),
                        imageSize,
                        ImVec2(u, v),
                        ImVec2(u + tw, v + th)
                    )) {
                        createdFrames[i].first = !createdFrames[i].first;
                    }

                    //Disalbe color sets
                    ImGui::PopStyleColor(2);

                    pos.x += currentTileWidth;
                    pos.y += currentTileHeight;
                }

                ImVec2 deltaPos;
                deltaPos.x = pos.x - startPos.x;
                deltaPos.y = pos.y - startPos.y;

                ImGui::SetCursorPos(ImVec2(windowPos.x, windowPos.y + deltaPos.y + padding));

                //Button for starting the flipbook
                if (ImGui::Button("Start FlipBook")) {
                    edittingFlipBook->StartFlipBook();
                }

                //Drawing a preview for the current frame
                auto& frames = edittingFlipBook->GetFrames();
                if (!frames.empty()) {
                    auto currentFrame = edittingFlipBook->GetCurrentFrame();

                    std::string imageButtonId = "##current_frame_image";
                    ImVec2 imageSize = ImVec2((float)currentFrame->GetSize().first, (float)currentFrame->GetSize().second);

                    ImGui::Image(
                        (ImTextureID)(intptr_t)params->GetTexture(),
                        imageSize,
                        ImVec2(currentFrame->GetUV().first, currentFrame->GetUV().second),
                        ImVec2(currentFrame->GetUV().first + currentFrame->GetTextureSize().first, currentFrame->GetUV().first + currentFrame->GetTextureSize().second));
                }

                if (ImGui::Button("Done")) {
                    showWindow = false;
                    edittingFlipBook = nullptr;
                }
            }
        }
    }

    ImGui::End();
}

/*
PURPOSE: Gets the instance of the class
*/
INTERFACEMANAGER_API WindowFlipBookEdit& WindowFlipBookEdit::GetInstance()
{
	static WindowFlipBookEdit window;
	return window;
}
