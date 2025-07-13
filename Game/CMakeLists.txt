##--- Game ---##

project(Game LANGUAGES CXX)

# Find GLSL validator
find_program(GLSLANG_VALIDATOR glslangValidator
    HINTS $ENV{VULKAN_SDK}/Bin
)
if(NOT GLSLANG_VALIDATOR)
    message(FATAL_ERROR "glslangValidator not found! Please install Vulkan SDK or add it to PATH.")
endif()

# Set the shader source and output directory
set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders)
set(SHADER_BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Shaders)
file(MAKE_DIRECTORY ${SHADER_BIN_DIR})

# Collect the shader source
file(GLOB_RECURSE SHADER_SRC_FILES
    RELATIVE ${SHADER_SRC_DIR}
    "${SHADER_SRC_DIR}/*.vert"
    "${SHADER_SRC_DIR}/*.frag"
)

# Convert .glsl to .spv
set(SPIRV_OUTPUTS)
foreach(SHADER_FILE IN LISTS SHADER_SRC_FILES)
    set(SRC "${SHADER_SRC_DIR}/${SHADER_FILE}")
    set(SPV "${SHADER_BIN_DIR}/${SHADER_FILE}.spv")
    add_custom_command(OUTPUT ${SPV}
        COMMAND ${GLSLANG_VALIDATOR} -V ${SRC} -o ${SPV}
        DEPENDS ${SRC}
        COMMENT "Compiling GLSL to SPIR-V: ${SHADER_FILE}"
        VERBATIM
    )
    list(APPEND SPIRV_OUTPUTS ${SPV})
endforeach()

# Custom function
add_custom_target(Shaders
    DEPENDS ${SPIRV_OUTPUTS}
)

# Game source files
file(GLOB_RECURSE GAME_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

# Build .exe
add_executable(${PROJECT_NAME} ${GAME_SOURCES})

# Turn the console window off in release build
target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
    $<$<CONFIG:Release>:/ENTRY:mainCRTStartup>
)

# Inlcude directory of the engine
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/Trinity/src
)

# Link the engine to the game
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Trinity
)

# Link shaders to the executable
add_dependencies(${PROJECT_NAME} 
    Shaders
)