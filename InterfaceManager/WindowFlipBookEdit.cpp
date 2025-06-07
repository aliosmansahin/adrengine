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
    FlipBookParams* params = dynamic_cast<FlipBookParams*>(edittingFlipBook->GetEntityParams());
    if (!params) {
        showWindow = false;
        return;
    }

    ImGui::Begin("Edit Flip Book", &showWindow, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);


    //Store entity id
    std::string textureIdStr = params->textureId;

    static char textureIdBuf[32] = "";

    if (textureIdBuf != textureIdStr) { // if the entity id changed
        strncpy_s(textureIdBuf, textureIdStr.c_str(), sizeof(textureIdBuf));
        textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
    }

    //Get texture Id
    if (ImGui::InputText("Texture Id", textureIdBuf, sizeof(textureIdBuf), ImGuiInputTextFlags_EnterReturnsTrue)) { //CHANGE IT TO OPTION PANEL THAT CONTAINS ALL TEXTURES
        if (textureIdBuf[0] == '\0') {// if the input is empty
            strncpy_s(textureIdBuf, params->name.c_str(), sizeof(textureIdBuf));
            textureIdBuf[sizeof(textureIdBuf) - 1] = '\0';
        }
        else {
            if (AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf)).get()) {
                params->textureId = std::string(textureIdBuf);
                params->texture = AssetDatabase::GetInstance().GetTexture(std::string(textureIdBuf))->texture;
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
                    params->textureId = texture->id;
                    params->texture = texture->texture;
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    //Draw the inspector texture for tile map
    if (!params->textureId.empty()) {
        auto& textures = AssetDatabase::GetInstance().GetTextures();
        auto textureIter = textures.find(params->textureId);
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
            //TODO: Write preview codes
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
