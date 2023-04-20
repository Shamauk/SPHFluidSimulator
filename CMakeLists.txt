cmake_minimum_required(VERSION 3.10)
project("SPH")

set (CMAKE_CXX_STANDARD 11)

FILE(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
FILE(GLOB_RECURSE HEADERS "src/*.h")

list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLSL} "src/main.cpp")

if(UNIX)
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Get the GLFW environment variable. There should be a CMakeLists.txt in the 
# specified directory.
SET(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.8)
IF(NOT GLFW_DIR)
    # The environment variable was not set
    SET(ERR_MSG "Please point the environment variable GLFW_DIR to the root directory of your GLFW installation.")
    IF(WIN32)
        # On Windows, try the default location
        MESSAGE(STATUS "Looking for GLFW in ${DEF_DIR_GLFW}")
        IF(IS_DIRECTORY ${DEF_DIR_GLFW})
            MESSAGE(STATUS "Found!")
            SET(GLFW_DIR ${DEF_DIR_GLFW})
        ELSE()
            MESSAGE(FATAL_ERROR ${ERR_MSG})
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR ${ERR_MSG})
    ENDIF()
ENDIF()
OPTION(GLFW_BUILD_EXAMPLES "GLFW_BUILD_EXAMPLES" OFF)
OPTION(GLFW_BUILD_TESTS "GLFW_BUILD_TESTS" OFF)
OPTION(GLFW_BUILD_DOCS "GLFW_BUILD_DOCS" OFF)
ADD_SUBDIRECTORY(${GLFW_DIR} ${GLFW_DIR}/build)
INCLUDE_DIRECTORIES(${GLFW_DIR}/include)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PRIVATE glfw)

# Get the GLEW environment variable.
SET(GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew-2.1.0)
IF(NOT GLEW_DIR)
    # The environment variable was not set
    SET(ERR_MSG "Please point the environment variable GLEW_DIR to the root directory of your GLEW installation.")
    IF(WIN32)
        # On Windows, try the default location
        MESSAGE(STATUS "Looking for GLEW in ${DEF_DIR_GLEW}")
        IF(IS_DIRECTORY ${DEF_DIR_GLEW})
            MESSAGE(STATUS "Found!")
            SET(GLEW_DIR ${DEF_DIR_GLEW})
        ELSE()
            MESSAGE(FATAL_ERROR ${ERR_MSG})
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR ${ERR_MSG})
    ENDIF()
ENDIF()
INCLUDE_DIRECTORIES(${GLEW_DIR}/include)
IF(WIN32)
    # With prebuilt binaries
    # Check for 32 vs 64 bit generator
    IF(NOT CMAKE_CL_64)
        MESSAGE(STATUS "Using 32Bit")
        TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} 
        ${GLEW_DIR}/lib/Release/Win32/glew32s.lib
)
    ELSE()
        MESSAGE(STATUS "Using 64Bit")
        TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} 
        ${GLEW_DIR}/lib/Release/x64/glew32s.lib
)
    ENDIF()
ELSE()
    # The pre-built doesn't exit. Maybe just ask the student to install it using the system package manager
    #TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${GLEW_DIR}/lib/libGLEW.a)
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PRIVATE ${GLEW_DIR}/lib/libGLEW.a)
ENDIF()

# Get GLM
SET(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm-0.9.9)
INCLUDE_DIRECTORIES(${GLM_DIR})

# Get imgui
SET(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui-1.89.5)
INCLUDE_DIRECTORIES(${IMGUI_DIR})

# Add ImGui source files
SET(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

# Create a static ImGui library
ADD_LIBRARY(imgui STATIC ${IMGUI_SRC})

# Link the ImGui library to your target application
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PRIVATE imgui)

# Use c++17
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# OS specific options and libraries
IF(WIN32)
    # -Wall produces way too many warnings.
    # -pedantic is not supported.
    # Disable warning 4996.
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PRIVATE opengl32.lib)

    SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
    SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "../resources")
ELSE()
    # Enable all pedantic warnings.
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
    IF(APPLE)
        # Add required frameworks for GLFW.
        TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} LINK_PRIVATE "-framework GLUT -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
    ELSE()
        #Link the Linux OpenGL library
        TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PRIVATE "GL")
    ENDIF()
ENDIF()

# Testing
# add_library(my_project_lib ${SOURCES})
# include_directories(src)
# enable_testing()
# add_executable(kernel_tests tests/kernel_tests.cpp)
# target_link_libraries(kernel_tests my_project_lib)
# add_test(NAME kernel_tests COMMAND kernel_tests)


