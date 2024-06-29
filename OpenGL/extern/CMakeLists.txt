set(
    LIBRARY_SOURCES
    "glad/glad.c"
    "imgui/imgui.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
)

set(
    LIBRARY_HEADERS
    "glad/glad.h"
    "stb/stb_image.h"
    "stb/stb_image_write.h"
    "stb/stb_image_resize.h"
    "stb/stb_include.h"
    "imgui/imgui.h"
    "imgui/backends/imgui_impl_opengl3.h"
)

set(
    LIBRARY_INCLUDES
    "./glad"
    "./stb"
    "./imgui"
    "./imgui/backends"
)

# STATIC/DYNAMIC LINKING OF LIBRARIES
add_library(
    ${EXTERN_LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS}
)

# Including the current directory for libraries
target_include_directories(
    ${EXTERN_LIBRARY_NAME} PUBLIC
    ${LIBRARY_INCLUDES}
)
