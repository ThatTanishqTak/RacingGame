# ------- ENGINE -------

project(Engine LANGUAGES CXX)

# Third-Party libraries

# GLFW
add_library(GLFW STATIC 
    IMPORTED
)
set_target_properties(GLFW PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/lib-vc2022/glfw3.lib
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include
)

# GLAD
add_library(GLAD STATIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLAD/src/glad.c>
)
target_include_directories(GLAD PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLAD/include>
)

# glm
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm>       
)

# Engine source files
file(GLOB_RECURSE ENGINE_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

# ImGui
set(IMGUI_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
)

# Build .lib
add_library(${PROJECT_NAME} STATIC 
    ${ENGINE_SOURCE}
    ${IMGUI_SOURCE}
)

# Include directory
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Game/src>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    IMGUI_IMPL_OPENGL_LOADER_GLAD    
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    GLFW
    GLAD
    glm
)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC opengl32)
endif()