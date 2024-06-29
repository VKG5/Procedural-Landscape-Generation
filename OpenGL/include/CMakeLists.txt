set(
    CUSTOM_LIB_NAMES
    custom_lib
)

add_subdirectory(${CUSTOM_LIB_NAMES})

## External libraries from Fetch Content
set(
    EXTERN_LIBS
    glm::glm
    assimp
    glfw
    # Vulkan-Headers
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
