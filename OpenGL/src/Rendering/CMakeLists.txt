set(
    EXEC_SOURCES
    # Main
    "Main.cpp"

    # Project Specific Files
    "Mesh.cpp"
    "GUI.cpp"
    "Model.cpp"
    "Scene.cpp"
    "Picking.cpp"

    # External Dependencies
    "commons/deps/imgui_impl_glfw.cpp"

    # Custom Algorithms
    "commons/algorithms/randomDistribute.cpp"

    # General - Sources that are common for all projects - MathFuncs.cpp, Camera.cpp, etc.
    "commons/Camera.cpp"
    "commons/DirectionalLight.cpp"
    "commons/Light.cpp"
    "commons/Material.cpp"
    "commons/MathFuncs.cpp"
    "commons/Movement.cpp"
    "commons/PointLight.cpp"
    "commons/Shader.cpp"
    "commons/Skybox.cpp"
    "commons/SpotLight.cpp"
    "commons/Texture.cpp"
    "commons/Utilities.cpp"
    "commons/Window.cpp"
)


# Adding the main file as an executable to our project
add_executable(
    ${EXECUTABLE_NAME}
    ${EXEC_SOURCES}
)

# We have inheritance modes - PUBLIC, PRIVATE, INTERFACE
target_link_libraries(
    ${EXECUTABLE_NAME} PUBLIC
    ${LIBRARY_NAME}
    ${EXTERN_LIBRARY_NAME}
)
