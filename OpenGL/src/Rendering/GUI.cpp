#include "GUI.h"

// Global variables for stride speed
float sliderSpeed = 0.01f;

GUI::GUI() {

}

void GUI::InitializeImGuiIO() {
    // Set all members to default or NULL values
    io = ImGui::GetIO();
    (void)io;
}

void GUI::initialize(GLFWwindow* mainWindow) {
    // Setup ImGUI Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    InitializeImGuiIO();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    // You can customize ImGuiIO settings here if needed
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

void GUI::newFrame() {
    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::elements(const std::string& shadingMode) {
    // Current Shading Mode
    // Set the font size
    ImGui::SetWindowFontScale(1.15f);
    ImGui::Text("Preview Mode:\n%s", shadingMode.c_str());

    // Material Preview
    // Spacing
    ImGui::Spacing();
    // Set the font size to default
    ImGui::SetWindowFontScale(1.0f);

    if (ImGui::BeginTabBar("Properties"))
    {
        // Material Properties
        if (ImGui::BeginTabItem("Materials")) {
            // Spacing
            ImGui::Spacing();

            // ImGui::Checkbox("Material Preview", &materialPreview);
            // // Checking different maps
            // // 1. Specular
            // // 2. Normal
            // if(materialPreview) {
            //     ImGui::Checkbox("Specular", &specularPreview);

            //     // Put the next element on the same line
            //     ImGui::SameLine();
            // }

            // File Path and Reload Terrain Button
            if(ImGui::BeginTabBar("Terrain Properties")) {
                if(ImGui::BeginTabItem("Terrain")) {
                    ImGui::InputText("Heightmap Path", heightmapPath, sizeof(heightmapPath));

                    // Upon pressing reload, set refreshTerrain to true
                    if(ImGui::Button("Reload")) {
                        refreshTerrain = true;
                    };

                    ImGui::DragFloat("Height", (float*)&landscapeHeight, sliderSpeed);

                    // End Current Tab Item
                    ImGui::EndTabItem();
                }

                // End Current Tab Bar
                ImGui::EndTabBar();
            }


            // Spacing
            ImGui::Spacing();
            ImGui::Checkbox("Wireframe", &wireframe);
            // Options based on wireframe
            if(wireframe) {
                ImGui::ColorEdit4("Wire Color", (float*)&wireframeColor);
            }

            // Spacing
            // You can also use - ImGui::Dummy(ImVec2(0.0f, 20.0f)); for custom height parameters (horizontal, vertical)
            ImGui::Spacing();
            ImGui::Checkbox("Heightmap", &shaded);

            // Options based on wireframe
            if(!shaded) {
                ImGui::ColorEdit4("Object Color", (float*)&objectColor);
            }

            // Spacing
            ImGui::Spacing();
            ImGui::Checkbox("Normal", &normalPreview);

            // Spacing
            ImGui::Spacing();
            ImGui::Checkbox("Hypsometric", &hypsometricPreview);

            if(hypsometricPreview) {
                ImGui::DragFloat("Base Height", (float*)&hypsometricHeight, sliderSpeed);
                ImGui::SliderInt("Height Multiplier", (int*)&hypsometricHeightMultiplier, 1, 100);
            }

            // Spacing
            ImGui::Spacing();
            if(ImGui::BeginTabBar("Object Properties")) {
                if(ImGui::BeginTabItem("Material")) {
                    ImGui::DragFloat("Specular", (float*)&specular, sliderSpeed, 0.0f, 1.0f);
                    ImGui::DragFloat("Shininess", (float*)&shininess, 1.0f, 0.0f, 4096.0f);
                    ImGui::DragFloat("Metalness", (float*)&metalness, sliderSpeed, 0.0f, 1.0f);

                    // End Current Tab Item
                    ImGui::EndTabItem();
                }

                // End Current Tab Bar
                ImGui::EndTabBar();
            }

            // End Current Tab Item
            ImGui::EndTabItem();
        }

        // Camera Properties
        if (ImGui::BeginTabItem("Camera")) {
            // Spacing
            ImGui::Spacing();

            // Setting camera properties
            ImGui::Checkbox("Perspective", &cameraIsPerspective);
            ImGui::SameLine();
            ImGui::Checkbox("Orthographic", &cameraIsOrthographic);

            if(cameraIsPerspective) {
                ImGui::DragFloat("Field of View", (float*)&cameraFOV, 1.0f, 0.0f);
            }

            else if(cameraIsOrthographic) {
                ImGui::DragFloat("Scale", (float*)&cameraOrthoScale, sliderSpeed);
            }

            ImGui::DragFloat("Near Clipping", (float*)&cameraNearClipping, sliderSpeed);
            ImGui::DragFloat("Far Clipping", (float*)&cameraFarClipping, sliderSpeed);
            ImGui::DragFloat("Camera Speed", (float*)&cameraSpeed, sliderSpeed);
            ImGui::DragFloat3("Camera Position", (float*)&cameraPos, sliderSpeed);

            ImGui::Spacing();
            ImGui::Checkbox("Rotate", &isCameraRotate);

            if(isCameraRotate) {
                ImGui::DragFloat("Radius", (float*)&cameraRotateRadius, sliderSpeed);
                ImGui::DragFloat("Rotation Speed", (float*)&cameraRotateSpeed, sliderSpeed);
            }

            // End Current Tab Item
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("World")) {
            // Spacing
            ImGui::Spacing();
            ImGui::ColorEdit4("Background Color", (float*)&bgColor);

            // TODO : Implement Skybox Controls
            ImGui::Spacing();
            ImGui::Text("Skybox");
            // Changing Skybox
            ImGui::SliderInt("Skybox", (int*)&skyboxIndex, 1, 6);
            ImGui::Checkbox("Skybox Active", &isSkyBox);
            ImGui::Checkbox("Draw Skybox", &drawSkybox);

            // End Current Tab Item
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Lighting")) {
            // Lighting
            // Spacing
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            ImGui::SetWindowFontScale(1.15f);
            ImGui::Text("Lighting");

            // Directional Light Controls
            ImGui::Spacing();
            ImGui::SetWindowFontScale(1.0f);
            ImGui::Text("Directional Light");
            ImGui::ColorEdit3("Color", directionalLightColor);
            ImGui::DragFloat3("Direction", directionalLightDirection, sliderSpeed, -360.0f, 360.0f, "%.3f");
            ImGui::DragFloat("Ambient", &directionalLightAmbient, sliderSpeed);
            ImGui::DragFloat("Diffuse", &directionalLightDiffuse, sliderSpeed);

            // End Current Tab Item
            ImGui::EndTabItem();
        }

        // End Current Tab Bar
        ImGui::EndTabBar();
    }
}

// Setters=============================================================================================================
// Camera
void GUI::setCameraIsPerspective(bool flag) {
    cameraIsPerspective = flag;
}

void GUI::setCameraIsOrthographic(bool flag) {
    cameraIsOrthographic = flag;
}

void GUI::setCameraFOV(float fieldOfView) {
    cameraFOV = fieldOfView;
}

void GUI::setCameraScale(float scale) {
    cameraOrthoScale = scale;
}

void GUI::setCameraClipping(float near, float far) {
    cameraNearClipping = near;
    cameraFarClipping = far;
}

void GUI::setCameraPosition(float x, float y, float z) {
    cameraPos[0] = x;
    cameraPos[1] = y;
    cameraPos[2] = z;
}

void GUI::setIsTerrainRefreshRequired(bool isRequired) {
    refreshTerrain = false;
}

void GUI::render(const std::string& shadingMode) {
    // Render ImGui elements here
    ImGui::Begin("Yumi");

    // Elements of the UI
    elements(shadingMode);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::warningMessage(const std::string& message) {
    ImGui::Begin("Warning");
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), message.c_str());
    ImGui::End();
}

void GUI::errorMessage(const std::string& message) {
    ImGui::Begin("Error");
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), message.c_str());
    ImGui::End();
}

void GUI::shutdown() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

GUI::~GUI() {
    shutdown();
}
