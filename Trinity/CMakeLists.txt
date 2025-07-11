project(Trinity LANGUAGES CXX)

# Third-Party libraries
# spdlog
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include>
)

file(GLOB_RECURSE ENGINE_SOURCE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_library(${PROJECT_NAME} STATIC ${ENGINE_SOURCE})

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
	spdlog
)

target_precompile_headers(${PROJECT_NAME} PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/trpch.h"
)

target_include_directories(${PROJECT_NAME} PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)