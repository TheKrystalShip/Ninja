cmake_minimum_required(VERSION 3.28)
project(ninja VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-but-set-variable")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCES src/App.cpp
            src/Box.cpp
            src/Capsule.cpp
            src/GameplayScreen.cpp
            src/main.cpp
            src/Player.cpp
)

add_executable(ninja ${SOURCES})

target_include_directories(ninja PUBLIC include ${TOASTER_INCLUDES} ${BOX2D_INCLUDES} ${SDL2_ttf_INCLUDE_DIR} ${SDL2_mixer_INCLUDE_DIRS})
target_link_libraries(ninja toaster box2d ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} SDL2_ttf::SDL2_ttf SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)

install(TARGETS ninja RUNTIME DESTINATION bin)

add_custom_command(TARGET ninja POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME      ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION   ${PROJECT_VERSION})
include(CPack)
