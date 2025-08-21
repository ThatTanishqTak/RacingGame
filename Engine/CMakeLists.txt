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

# Build .lib
add_library(${PROJECT_NAME} STATIC 
    ${ENGINE_SOURCE}
)

# Include directory
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
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