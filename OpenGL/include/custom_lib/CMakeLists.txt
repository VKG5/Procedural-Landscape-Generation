set(
    LIBRARY_SOURCES
)

set(
    LIBRARY_HEADERS
    "Mesh.h"
    "Bones.h"
    "Shader.h"
    "Window.h"
    "Camera.h"
    "deps/imgui_impl_glfw.h"
    "GUI.h"
    "Texture.h"
    "Light.h"
    "Utilities.h"
    "Material.h"
    "DirectionalLight.h"
    "PointLight.h"
    "SpotLight.h"
    "Model.h"
    "Skybox.h"
    "Movement.h"
    "algorithms/randomDistribute.h"
    "MathFuncs.h"
    "Scene.h"
    "Picking.h"
)

set(
    LIBRARY_INCLUDES
    "./"
    "./deps"
    "./algorithms"
)

set(
    EXTERN_LIBS
)

# STATIC/DYNAMIC LINKING OF LIBRARIES
add_library(
    ${LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS}
)

# Including the current directory for libraries
target_include_directories(
    ${LIBRARY_NAME} PUBLIC
    ${LIBRARY_INCLUDES}
)

# Linking the external libraries that we got from FetchContent
# Linking the libraries to our custom library (LIBRARY_NAME)
# We use project_name::library_name (Both can be found in the
# CMakeLists.txt for the library) for FetchContent libs
# For manual libs, just use the project_name
target_link_libraries(
    ${LIBRARY_NAME} PUBLIC
    ${EXTERN_LIBS}
)
