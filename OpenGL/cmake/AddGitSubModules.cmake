# We're making a custom function
# Parameters:
# 1. Function name - We'll call the function later using this
# 2. Arguments - Any number of arguments you want
function(add_git_submodules dir)
    # Checking if the GIT package is present on your system or not
    find_package(Git REQUIRED)

    # Checking for a CMake File in the project (In case the manually
    # added library is a CMake Library)
    if(NOT EXISTS ${dir}/CMakeLists.txt)
        # Executing some Git commands
        execute_process(COMMAND ${GIT_EXECUTABLE}
            submodule update --init --recursive --${dir}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )

    endif()
    # Adding the subdirectory of the library to the project
    add_subdirectory(${dir})

endfunction(add_git_submodules)
