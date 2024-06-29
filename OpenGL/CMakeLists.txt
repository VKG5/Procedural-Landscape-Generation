## Steps to follow - Generate the project
# Create source and CMakeLists
# 1) mkdir build
# 2) cd build
# 3) cmake .. -> Generating the build files/Configuring the project
# 4) cmake --build . -> Command to build all targets
# 5) ./Executable

## Steps to follow - Update project
# 1) cd build
# 2) cmake .

## Setting the generator in Terminal to a specific version
# cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=17 -S path/to/source -B path/to/build

cmake_minimum_required(VERSION 3.24)

project(
    HelloWorld
    VERSION 1.0.0
    LANGUAGES C CXX
    )

# Setting the language version
set(CXX_STANDARD_REQUIRED 17)

# Making it so the language is enforced
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For external libraries
set(CMAKE_CXX_EXTENSIONS OFF)

# Setting the Executable and Library Name
set(EXTERN_LIBRARY_NAME Library_External)
set(EXECUTABLE_NAME Executable)
set(LIBRARY_NAME Library)

# Add external libraries manually
# Setting the Cmake modules path to our custom path
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

## Adding extra modules
include(FetchContent)
include(AddGitSubmodules)

# Adding git libraries with inbuilt function
FetchContent_Declare(
    Vulkan-Headers
    GIT_REPOSITORY "https://github.com/KhronosGroup/Vulkan-Headers"
    GIT_TAG v1.3.204
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(Vulkan-Headers)

FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG 0.9.9.8
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG 3.3.8
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY "https://github.com/assimp/assimp"
    GIT_TAG v5.3.1
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(assimp)

# Calling the function
#add_git_submodules(extern/glew)

# Since src (Application) is dependent on include (Custom Libraries)
# We include them in a reverse order, otherwise
# The application will throw an error while building
add_subdirectory(extern)
add_subdirectory(include)
add_subdirectory(src)
