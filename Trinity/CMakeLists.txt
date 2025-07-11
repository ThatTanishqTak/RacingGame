##--- Engine ---##

project(Trinity LANGUAGES CXX)

# Engine dependencies
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

find_package(spdlog REQUIRED)

# Include dir
file(GLOB_RECURSE ENGINE_SOURCE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# Output .lib
add_library(${PROJECT_NAME} STATIC 
	${ENGINE_SOURCE}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
        spdlog::spdlog
        glfw
        Vulkan::Vulkan
)

target_precompile_headers(${PROJECT_NAME} PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/trpch.h"
)

target_include_directories(${PROJECT_NAME} PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)