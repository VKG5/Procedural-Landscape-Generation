#include "Scene.h"

// Using initializer list since the type is primitive GLuint and we need the values initialized upon creation of the object
Scene::Scene(Window& window, GLuint s)  :
                uniformProjection(0), uniformModel(0), uniformView(0), uniformEyePosition(0),
                uniformSpecularIntensity(0), uniformShininess(0), uniformMetalness(0),
                uniformshadingModel(0),
                uniformIsShaded(0), uniformIsWireframe(0), uniformObjectColor(0), uniformWireframeColor(0),
                uniformMaterialPreview(0), uniformSpecularPreview(0), uniformNormalPreview(0),
                uniformDiffuseTexture(0), uniformAmbientOcclusionTexture(0), uniformSpecularTexture(0), uniformNormalTexture(0),
                uniformNoiseTexture(0),
                uniformEnvMapping(0), uniformSkybox(0), uniformBackgroundColor(0),
                uniformIsReflection(0), uniformIsRefraction(0), uniformIOR(0), uniformFresnelReflectance(0), uniformDispersion(0),
                uniformNormalStrength(0), uniformSpecularStrength(0),
                mainWindow(window), seed(s) {
    // Initialize any resources here
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.125f, 3.0f);

    // Projection Matrix===============================================================================================
    projection = camera.calculatePerspectiveProjectionMatrix(mainWindow.getBufferWidth(), mainWindow.getBufferHeight());

    // ImGUI===========================================================================================================
    mainGUI.initialize(mainWindow.getWindow());

    // Generating random points - Pre-Loading for faster rendering
    generateRandomPoints(randomPoints, gridSize, pointSize, numPoints, seed);
    generateRandomScales(randomScales, randomPoints.size(), seed);
    generateRandomHeights(randomHeights, randomPoints.size(), seed);
}

void Scene::createPlane(const float floorSize, const float floorUV) {
    // Floor indices
    unsigned int indicesFloor[] = {
        0, 1, 2,
        1, 2, 3
    };

    // Floor vertices
    GLfloat verticesFloor[] = {
        // x, y, z                      u, v              Nx, Ny, Nz              Tx, Ty, Tz
        -floorSize, 0.0f, -floorSize,   0.0f, 0.0f,       0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f,
        floorSize, 0.0f, -floorSize,    floorUV, 0.0f,    0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f,
        -floorSize, 0.0f, floorSize,    0.0f, floorUV,    0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f,
        floorSize, 0.0f, floorSize,     floorUV, floorUV, 0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 0.0f
    };

    Mesh* floor = new Mesh();
    floor->createMesh(verticesFloor, indicesFloor, 44, 6);

    // Adding to our meshlist
    meshList.push_back(floor);
}

void Scene::createShaders(const std::filesystem::path& currentSourceDir) {
    // Shader 1
    // Vertex Shader
    // Uniform - Global to shader, not associated with a particular vertex
    // Bind data to uniform to get location
    std::string vertexShaderPath = returnPath(currentSourceDir, "Shaders/BRDF_Normals.vert");
    static const char* vertexShader = vertexShaderPath.c_str();

    // Fragment Shader
    std::string fragmentShaderPath = returnPath(currentSourceDir, "Shaders/BRDF_Normals.frag");
    static const char* fragmentShader = fragmentShaderPath.c_str();

    Shader* shader1 = new Shader();
    shader1->createFromFiles(vertexShader, fragmentShader);

    shaderList.push_back(shader1);
}

void Scene::createLights() {
    // Setting up lights
    mainLight = DirectionalLight( 1.0f, 0.85f, 0.65f,
                                  0.5f, 0.35f,
                                  2.0f, -1.0f, -2.0f );

    // Point Lights
    pointLights[0] = PointLight( 1.0f, 0.0f, 0.0f,
                                 0.35f, 0.25,
                                 -2.0f, 0.0f, 0.0f,
                                 0.3f, 0.2f, 0.1f );
    pointLightCount++;

    pointLights[1] = PointLight( 0.0f, 1.0f, 0.0f,
                                 0.35f, 0.25,
                                 0.0f, 0.0f, 0.0f,
                                 0.3f, 0.1f, 0.1f );
    pointLightCount++;

    pointLights[2] = PointLight( 0.0f, 0.0f, 1.0f,
                                 0.35f, 0.25,
                                 2.0f, 0.0f, 1.0f,
                                 0.3f, 0.1f, 0.1f );
    pointLightCount++;

    // This is our torch
    spotLights[0] = SpotLight(  1.0f, 1.0f, 1.0f,
                                0.25f, 1.0f,
                                0.0f, 0.0f, 0.0f,
                                0.0f, -1.0f, 0.0f,
                                1.0f, 0.0f, 0.0f,
                                20.0f );
    spotLightCount++;

    // Skybox==========================================================================================================
    // Loading default skyboxes
    mainSkybox = std::make_unique<Skybox>();
    mainSkybox->loadDefaultSkyboxes();
}

void Scene::createTextures() {
    // Setting up Textures=============================================================================================
    // Choices - Parameter : Give the kind of loading you want to do for the particular texture
    // 1. No Interpolation and No MIP Maps
    // 2. No Interpolation and MIP Maps
    // 3. Interpolation and MIP Maps Near
    // 4. Intepolation and MIP Maps Interpolation
    whiteTexture = Texture("D:/Programs/C++/Rendering/OpenGL/src/Rendering/Textures/Default/white.jpg");
    whiteTexture.loadTexture();
    brickTexture = Texture("D:/Programs/C++/Rendering/OpenGL/src/Rendering/Textures/Default/brickHi.png");
    brickTexture.loadTexture();

    // Generated Noise Texture
    // Parameters - Width, Height, Channels = 3 (Use 3 channels - RGB)
    // noiseTexture = Texture();
    // noiseTexture.generateRandomTexture(2048, 2048, 3);
}

void Scene::createMaterials() {
    // Setting up Materials============================================================================================
    // Make the second parameter (Shine) to be powers of 2
    shinyMat = Material(1.0f, 256, 0.1f);
    roughMat = Material(0.5f, 64, 0.8f);
    extraRoughMat = Material(0.125f, 2, 1.0f);
    extraShinyMat = Material(1.0f, 1024, 0.0125f);
}

void Scene::setupScene(const std::filesystem::path& currentSourceDir) {
    // Create Shaders
    createShaders(currentSourceDir);

    // Creating Lights and default Skyboxes
    createLights();

    // Loading and creating Textures
    createTextures();

    // Creating Materials
    createMaterials();

    // Loading and creating Objects/Models
    // The plane is necessary for PCG
    loadObjects();
}

void Scene::getUniformsFromShader(Shader * shader) {
    // Binding the program
    shader->useShader();

    uniformModel = shader->getModelLocation();
    uniformProjection = shader->getProjectionLocation();
    uniformView = shader->getViewLocation();

    // Specular Light
    uniformEyePosition = shader->getEyePositionLocation();
    uniformSpecularIntensity = shader->getSpecularIntensityLocation();
    uniformShininess = shader->getShininessLocation();
    uniformMetalness = shader->getMetalnessLocation();

    // Getting Shading Mode
    uniformshadingModel = shader->getShadingModelLocation();

    /*
    Getting UI Elements
    */
    // Object Properties
    uniformIsShaded = shader->getIsShadedLocation();
    uniformIsWireframe = shader->getIsWireframeLocation();
    uniformWireframeColor = shader->getWireframeColourLocation();
    uniformObjectColor = shader->getObjectColorLocation();
    uniformEnvMapping = shader->getEnvMappingLocation();
    uniformSkybox = shader->getSkyboxLocation();
    uniformBackgroundColor = shader->getBackgroundColourLocation();

    // Material Preview - Maps
    uniformMaterialPreview = shader->getMaterialPreviewLocation();
    uniformSpecularPreview = shader->getSpecularPreviewLocation();
    uniformNormalPreview = shader->getNormalPreviewLocation();

    // Material Properties
    uniformIsReflection = shader->getIsReflectionLocation();
    uniformIsRefraction = shader->getIsRefractionLocation();
    uniformIOR = shader->getIORLocation();
    uniformFresnelReflectance = shader->getFresnelReflectance();
    uniformDispersion = shader->getDispersionLocation();
    uniformNormalStrength = shader->getNormalStrengthLocation();
    uniformSpecularStrength = shader->getSpecularStrengthLocation();

    // Texture
    uniformDiffuseTexture = shader->getMainTextureLocation();
    uniformSpecularTexture = shader->getSpecularTextureLocation();
    uniformAmbientOcclusionTexture = shader->getAmbientOcclusionTextureLocation();
    uniformNormalTexture = shader->getNormalTextureLocation();
}

void Scene::setUniformsForShader(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, Shader * shader) {
    // Binding the texture to correct texture units
    shader->setTexture(uniformDiffuseTexture, 0);
    shader->setTexture(uniformSpecularTexture, 1);
    shader->setTexture(uniformNormalTexture, 2);

    // TODO : Intergrate this to work like a proper roughness map
    // uniformNoiseTexture = shader.getNoiseTextureLocation();
    // shader.setTexture(uniformNoiseTexture, 1);

    // Set the position of our moveable spot light w.r.t. the camera
    // glm::vec3 lowerLight = camera.getCameraPosition();
    // lowerLight.y -= 0.369f;
    // Getting torch control
    // spotLights[0].setFlash(lowerLight, camera.getCameraDirection());

    // Lights
    // Directional Light
    mainLight.setDirLight( mainGUI.getDirectionalLightColor()[0],
                           mainGUI.getDirectionalLightColor()[1],
                           mainGUI.getDirectionalLightColor()[2],
                           mainGUI.getDirectionalLightAmbient(),
                           mainGUI.getDirectionalLightDiffuse(),
                           mainGUI.getDirectionalLightDirection()[0],
                           mainGUI.getDirectionalLightDirection()[1],
                           mainGUI.getDirectionalLightDirection()[2] );

    shader->setDirectionalLight(&mainLight);

    // Currently disabling the Point and Spot lights based off a boolean
    // Point Lights
    if(mainGUI.getIsPointLights()) {
        shader->setPointLight(pointLights, pointLightCount);
    }

    else {
        shader->setPointLight(pointLights, 0);
    }

    // Spot Lights
    if(mainGUI.getIsSpotLights()) {
        shader->setSpotLight(spotLights, spotLightCount);
    }

    else {
        shader->setSpotLight(spotLights, 0);
    }

    // Binding to uniforms in the shader
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x,
                                    camera.getCameraPosition().y,
                                    camera.getCameraPosition().z);

    // Setting the shading model
    glUniform1i(uniformshadingModel, shadingModel);

    // Setting the triggers from UI Elements
    // Material Preview
    glUniform1i(uniformMaterialPreview, mainGUI.getMaterialPreview());
    glUniform1i(uniformSpecularPreview, mainGUI.getSpecularPreview());
    glUniform1i(uniformNormalPreview, mainGUI.getNormalPreview());

    // Object Properties
    glUniform1i(uniformIsWireframe, mainGUI.getIsWireframe());
    glUniform1i(uniformIsShaded, mainGUI.getIsShaded());

    glUniform4f(uniformObjectColor, mainGUI.getObjectColor().x,
                                    mainGUI.getObjectColor().y,
                                    mainGUI.getObjectColor().z,
                                    mainGUI.getObjectColor().w );

    glUniform4f(uniformWireframeColor, mainGUI.getWireframeColor().x,
                                       mainGUI.getWireframeColor().y,
                                       mainGUI.getWireframeColor().z,
                                       mainGUI.getWireframeColor().w );

    glUniform1i(uniformEnvMapping, mainGUI.getIsEnvMapping());
    glUniform1i(uniformSkybox, mainGUI.getIsSkyBox());
    glUniform3f(uniformBackgroundColor, mainGUI.getBackgroundColor().x,
                                        mainGUI.getBackgroundColor().y,
                                        mainGUI.getBackgroundColor().z);

    // Material Properties
    glUniform1i(uniformIsReflection, mainGUI.getIsReflection());
    glUniform1i(uniformIsRefraction, mainGUI.getIsRefraction());
    glUniform1f(uniformIOR, mainGUI.getIOR());
    glUniform1f(uniformFresnelReflectance, mainGUI.getFresnelReflectance());
    glUniform1f(uniformDispersion, mainGUI.getDispersion());
    glUniform1f(uniformNormalStrength, mainGUI.getNormalStrength());
    glUniform1f(uniformSpecularStrength, mainGUI.getSpecularStrength());
}

// Scene Properties====================================================================================================
// Global parameter for rotating the objects
float rotationAngle = 0.0f;
float step = 0.05f;
float size = 12.0f;

// Contains a basic PCG Scene that can be rendered if enabled
void Scene::renderPCGElements() {
    // Objects=========================================================================================================
    // Grounds
    if(rotationAngle < 360.0f) {
        rotationAngle += step;
    }

    else {
        rotationAngle = 0.0f;
    }

    // Floor
    glm::mat4 floor = glm::mat4(1.0f);

    // TRS
    // To rotate around origin
    // floor = glm::rotate(floor, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    floor = glm::translate(floor, glm::vec3(mainGUI.getFloorOffset()[0],
                                            mainGUI.getFloorOffset()[1],
                                            mainGUI.getFloorOffset()[2]));
    floor = glm::scale(floor, glm::vec3(mainGUI.getFloorScale()[0],
                                        mainGUI.getFloorScale()[1],
                                        mainGUI.getFloorScale()[2]));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(floor));
    brickTexture.useTexture();
    roughMat.useMaterial(uniformSpecularIntensity, uniformShininess, uniformMetalness);

    // Failsafe
    if(meshList.size()) {
        meshList[0]->renderMesh();
    }

    else {
        createPlane();
        meshList[0]->renderMesh();
    }

    // Buildings=======================================================================================================
    // Randomly placing the buildings
    for (size_t i=0; i < randomPoints.size(); i++) {
        const auto& point = randomPoints[i];
        const auto& scale = randomScales[i];
        const auto& height = randomHeights[i];

        for (int j = 0; j < height + 1; j++) {
            float heightOffset = 0.0f;
            // TODO : Optimize
            if (point.first % 3 == 0) {
                heightOffset = 2.64f;
            }

            else if (point.first % 3 == 1) {
                heightOffset = 3.29f;
            }

            else {
                continue;
            }

            glm::mat4 buil = glm::mat4(1.0f);
            // Rotating so that it rotates along the origin then transalte and scale
            // buil = glm::rotate(buil, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

            // TRS
            buil = glm::translate(buil, glm::vec3(point.first, j * (heightOffset * scale.y), -point.second));
            buil = glm::scale(buil, scale);

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(buil));
            roughMat.useMaterial(uniformSpecularIntensity, uniformShininess, uniformMetalness);

            // Randomly pick a model
            // int modelIndex = modelDistribution(gen);

            // Determine which model to render based on some criteria (e.g., point coordinates)
            if (point.first % 3 == 0) {
                building0->renderModel();
            }

            else if (point.first % 3 == 1) {
                building1->renderModel();
            }

            else {
                continue;
            }
        }
    }
}

void Scene::generalElements(glm::mat4& projectionMatrix, glm::mat4& viewMatrix) {
    // Clear window
    glClearColor( mainGUI.getBackgroundColor().x,
                  mainGUI.getBackgroundColor().y,
                  mainGUI.getBackgroundColor().z,
                  mainGUI.getBackgroundColor().w );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Checking for Skybox parameter in UI
    if(mainGUI.getIsSkyBox() && mainGUI.getDrawSkyBox()) {
        // Drawing the Skybox before everything else
        // Checking Skybox index
        mainSkybox->getDefaultSkyboxes()[mainGUI.getSkyboxIndex() - 1]->drawSkybox(viewMatrix, projectionMatrix);
    }

    // UI - Render Frame
    mainGUI.newFrame();

    // Toggle cursor mode on pressing the 'C' key
    if (mainWindow.getKeys()[GLFW_KEY_C]) {
        // Altering the state
        cursorDisabled = !cursorDisabled;

        // Debugging
        // printf("Pressed C!\n%i", cursorDisabled);

        mainWindow.changeState(cursorDisabled);

        // Preventing multiple inputs
        mainWindow.getKeys()[GLFW_KEY_C] = false;
    }

    // Updating the camera speed
    camera.updateMoveSpeed(mainGUI.getCameraSpeed());

    // Setting camera using GUI
    camera.setPosition(glm::vec3(mainGUI.getCameraPosition()[0], mainGUI.getCameraPosition()[1], mainGUI.getCameraPosition()[2]));

    // Projection Matrix
    camera.setCameraParameters( mainGUI.getCameraFOV(),
                                mainGUI.getCameraScale(),
                                mainGUI.getCameraNearClipping(),
                                mainGUI.getCameraFarClipping() );

    // Numpad 5 for switching between Ortho and Perspective
    if(mainWindow.getKeys()[GLFW_KEY_KP_5]) {
        if(mainGUI.getCameraIsPerspective()) {
            mainGUI.setCameraIsPerspective(false);

            // Turn off Anaglyph for Orthographic view
            mainGUI.setIsAnaglyph(false);
            mainGUI.setCameraIsOrthographic(true);
        }

        else {
            mainGUI.setCameraIsOrthographic(false);

            // Turn off Anaglyph while switching back to Perspective
            mainGUI.setIsAnaglyph(false);
            mainGUI.setCameraIsPerspective(true);
        }

        // Reset the rendering mode to Normal on each switch
        renderingMode = 0;
        mainWindow.getKeys()[GLFW_KEY_KP_5] = false;
    }

    // Setting Rendering Mode - 'M'
    if(mainWindow.getKeys()[GLFW_KEY_M]) {
        renderingMode = (renderingMode + 1) % 3;

        // Debugging
        // printf("%i", renderingMode);

        // Default Rendering
        if(renderingMode == 0) {
            mainGUI.setIsAnaglyph(false);
            mainGUI.setIsToedInRendering(false);
            mainGUI.setIsAsymmetricFrustumRendering(false);
        }

        // Anaglyph - Toed-In Rendering
        else if(renderingMode == 1) {
            mainGUI.setIsAnaglyph(true);
            mainGUI.setIsToedInRendering(true);
            mainGUI.setIsAsymmetricFrustumRendering(false);
        }

        // Anaglyph - Asymmetric Frustum
        else {
            mainGUI.setIsAnaglyph(true);
            mainGUI.setIsToedInRendering(false);
            mainGUI.setIsAsymmetricFrustumRendering(true);
        }

        // Stopping multiple inputs from the same key
        mainWindow.getKeys()[GLFW_KEY_M] = false;
    }

    // Switching to Perspective
    if(mainGUI.getCameraIsPerspective()) {
        mainGUI.setCameraIsOrthographic(false);
        projectionMatrix = camera.calculatePerspectiveProjectionMatrix(mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
    }

    // Switching to Orthographic
    if(mainGUI.getCameraIsOrthographic()) {
        mainGUI.setCameraIsPerspective(false);
        projectionMatrix = camera.calculateOrthographicProjectionMatrix();
    }

    // Handling input events exclusive to the GLFW Window
    if(!mainGUI.getIO().WantCaptureMouse) {
        // If the cursor is disabled, don't update the camera
        if(cursorDisabled) {
            // Camera Key Controls
            camera.keyControl(mainWindow.getKeys(), deltaTime);
            camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

            mainGUI.setCameraPosition( camera.getCameraPosition().x,
                                       camera.getCameraPosition().y,
                                       camera.getCameraPosition().z );
        }

        // Rotate around origin
        if(mainGUI.getIsCameraRotating()) {
            if(cameraAngle < 360.0f) {
                cameraAngle += deltaTime * mainGUI.getCameraRotationSpeed();
            }

            else {
                cameraAngle = 0.0f;
            }

            camera.setPosition(glm::vec3(radius * glm::cos(cameraAngle) + mainGUI.getCameraPosition()[0],
                                         mainGUI.getCameraPosition()[1],
                                         radius * glm::sin(cameraAngle) + mainGUI.getCameraPosition()[2]));
        }

        // Toggling Spot Light on pressing L
        if(mainWindow.getKeys()[GLFW_KEY_L]) {
            // Debugging
            // printf("Pressed L!");

            spotLights[0].toggle();
            mainWindow.getKeys()[GLFW_KEY_L] = false;
        }

        if(mainWindow.getKeys()[GLFW_KEY_B]) {
            // Debugging
            // printf("Pressed B!\n");

            shadingModel = (shadingModel + 1) % MAX_SHADING_MODELS;

            // Debugging
            // std::cout<<shadingModel<<"\n";

            // To prevent multiple clicks in this loop
            mainWindow.getKeys()[GLFW_KEY_B] = false;
        }
    }

    // Switching modes - Wireframe/Normal
    if(mainGUI.getIsWireframe()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Updating the Procedural Content on button press
    if(mainGUI.getUpdate()) {
        // Debugging
        // printf("In update!\n");
        // printf("Grid : %i\nPoint : %i\nNum : %i\nSeed : %i\n",mainGUI.getGridSize(),
        //                                                     mainGUI.getPointSize(),
        //                                                     mainGUI.getNumPoints(),
        //                                                     mainGUI.getSeed() );

        generateRandomPoints(randomPoints,
                             mainGUI.getGridSize(),
                             mainGUI.getPointSize(),
                             mainGUI.getNumPoints(),
                             mainGUI.getSeed() );

        generateRandomScales(randomScales, randomPoints.size(), seed);
        generateRandomHeights(randomHeights, randomPoints.size(), seed);

        // To prevent multiple clicking
        mainGUI.setUpdate(false);
    }
}

// Calculate Toed-in or Asymmetric Frustum Anaglyph
void Scene::calculateAnaglyph(glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix) {
    /* Toed-In Method of Anaglyphical rendering
    In this method, we change the view matrix to point towards a custom target and offset the
    Render passes based off of the Inter Ocular Distance (IOD) and Convergence Distance (CD)
    * Only works for perspective renders
    */
    //Red pass - Left Eye
    glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Checking for channel flip
    bool leftEye = mainGUI.getIsAnaglyphChannelsFlippedToed() ? false : true;
    bool rightEye = mainGUI.getIsAnaglyphChannelsFlippedFrustum() ? false : true;

    if(mainGUI.getIsToedInRendering()) {
        // Modifying the view matrix for left eye here
        viewMatrix = camera.calculateViewMatrix(leftEye,
                                                mainGUI.getInterOcularDistance(),
                                                mainGUI.getCovergenceDistance());
    }

    else if(mainGUI.getIsAsymmetricFrustumRendering()) {
        // Modifying the projection matrix for left eye here
        projectionMatrix = camera.calculateAsymmetricFrustum(rightEye,
                                                             mainGUI.getInterOcularDistance(),
                                                             mainGUI.getCovergenceDistance(),
                                                             mainWindow.getBufferWidth(),
                                                             mainWindow.getBufferHeight());
    }

    // Setting Uniforms for a shader
    getUniformsFromShader(shaderList[0]);
    setUniformsForShader(projectionMatrix, viewMatrix, shaderList[0]);

    // Rendering the scene
    renderScene();

    // Cyan pass - Right Eye
    glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);

    if(mainGUI.getIsToedInRendering()) {
        // Modifying the view matrix for right eye here
        viewMatrix = camera.calculateViewMatrix(!leftEye,
                                                mainGUI.getInterOcularDistance(),
                                                mainGUI.getCovergenceDistance());
    }

    else if(mainGUI.getIsAsymmetricFrustumRendering()) {
        // Modifying the projection matrix for right eye here
        projectionMatrix = camera.calculateAsymmetricFrustum(!rightEye,
                                                             mainGUI.getInterOcularDistance(),
                                                             mainGUI.getCovergenceDistance(),
                                                             mainWindow.getBufferWidth(),
                                                             mainWindow.getBufferHeight());
    }

    // Setting Uniforms for a shader
    getUniformsFromShader(shaderList[0]);
    setUniformsForShader(projectionMatrix, viewMatrix, shaderList[0]);

    // Rendering the scene
    renderScene();

    // Resetting the color pass to render all colors
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}


// Custom Scene definition is here!====================================================================================
void Scene::loadObjects() {
    // Loading Models==================================================================================================
    // Default PCG Models
    building0 = new Model();
    building0->loadModel("D:/Programs/C++/Rendering/OpenGL/src/Rendering/Models/buildings.obj");

    building1 = new Model();
    building1->loadModel("D:/Programs/C++/Rendering/OpenGL/src/Rendering/Models/buildings_2.obj");

    cube = new Model();
    cube->loadModel("D:/Programs/C++/Rendering/OpenGL/src/Rendering/Models/cube.obj");

    monkey = new Model();
    monkey->loadModel("D:/Programs/C++/Rendering/OpenGL/src/Rendering/Models/monkey.obj");
}

void Scene::renderScene() {
    // If we want to render PCG Elements
    if(mainGUI.getIsPCG()) {
        renderPCGElements();
    }

    else {
        if(rotationAngle < 360.0f) {
            rotationAngle += step;
        }

        else {
            rotationAngle = 0.0f;
        }

        glm::mat4 base = glm::mat4(1.0f);
            // TRS
            base = glm::translate(base, glm::vec3(0.0f, 2.0f, 0.0f));
            base = glm::rotate(base, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            base = glm::scale(base, glm::vec3(1.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(base));

        monkey->updateMaterialProperties(mainGUI.getSpecular(), mainGUI.getShininess(), mainGUI.getMetalness());
        monkey->setMaterialUniforms(uniformSpecularIntensity, uniformShininess, uniformMetalness);
        monkey->renderModel();

        // Debugging
        // ImGui::Text("%i, %i", mainWindow.getBufferWidth(), mainWindow.getBufferHeight());

        cube->setInitialTransformMatrix();
            // TRS
            cube->updateTranslation( camera.getRayHitCoords(mainWindow.getXPos(),
                                                            mainWindow.getYPos(),
                                                            mainWindow.getBufferWidth(),
                                                            mainWindow.getBufferHeight()) );
        cube->updateTransform();

        extraRoughMat.useMaterial(uniformSpecularIntensity, uniformShininess, uniformMetalness);
        cube->renderModel(uniformModel);
    }
}

// Render Pass - Renders all data in the scene=========================================================================
void Scene::renderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    // General Pass
    // Includes the general elements of the program
    /*
    1. Background Color, Depth Pass
    2. Skybox
    3. GUI - New Frame
    4. Camera Controls
    5. Wireframe
    */
    // Always called
    generalElements(projectionMatrix, viewMatrix);

    // Anaglyph Rendering
    if(mainGUI.getIsAnaglyph()) {
        calculateAnaglyph(projectionMatrix, viewMatrix);
    }

    else {
        // Setting Uniforms for a shader
        getUniformsFromShader(shaderList[0]);
        setUniformsForShader(projectionMatrix, viewMatrix, shaderList[0]);

        // Rendering the scene
        renderScene();
    }

    // Drawing the UI
    setShadingModeName(mainGUI, shadingModel, shadingMode);
    mainGUI.render(shadingMode);
}

void Scene::update(float time) {
    deltaTime = time;

    // Handles the rendering of each elements - UI, GLFW, Objects, etc.
    renderPass(projection, camera.calculateViewMatrix());

}

Scene::~Scene() {
    // Cleanup for meshList
    for (auto* mesh : meshList) {
        delete mesh;
    }
    meshList.clear();

    // Cleanup for shaderList
    for (auto* shader : shaderList) {
        delete shader;
    }
    shaderList.clear();

    // Manual deletion for smart pointers isn't necessary
}
