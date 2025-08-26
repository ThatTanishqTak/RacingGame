# ------- GAME -------

project(Game LANGUAGES CXX)

add_compile_definitions(MANAGEMENT_MODE)

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
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/Engine/src
)

# Link the engine to the game
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Engine
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets"
    COMMENT "Copying ${CMAKE_CURRENT_SOURCE_DIR}/Assets to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets"
)