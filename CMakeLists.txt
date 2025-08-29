# ------- WORKSPACE -------

cmake_minimum_required(VERSION 3.28)
project(RacingGame)

# Enforce the use of C++17 across the workspace
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to disable heavy 3D track rendering for management mode
option(ENABLE_MANAGEMENT_MODE "Disable 3D track rendering" ON)
if(ENABLE_MANAGEMENT_MODE)
        add_compile_definitions(MANAGEMENT_MODE)
endif()

# Add compiler options
if(MSVC)
	add_compile_options(/MP)
	#add_compile_options(/utf-8)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ARCH "x64")
else()
	set(ARCH "x86")
endif()

# Create custom output directory
set(OUTPUT_DIR ${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${ARCH})

# Set custom output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/${PROJECT_NAME})

add_subdirectory(Engine)
add_subdirectory(Game)