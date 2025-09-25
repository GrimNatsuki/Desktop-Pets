cmake_minimum_required(VERSION 3.21)
project(Desktop-Pets LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED CONFIG)

add_executable(desktop-pets 
    src/main.cpp
    src/PetEngine.cpp
    src/Timer.cpp)

target_include_directories(desktop-pets PRIVATE inc)

target_link_libraries(desktop-pets PRIVATE 
    SDL3::SDL3
    SDL3_image::SDL3_image
    nlohmann_json::nlohmann_json)
target_link_options(desktop-pets PRIVATE -static-libgcc -static-libstdc++)

add_custom_command(TARGET desktop-pets POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tex
        $<TARGET_FILE_DIR:desktop-pets>/tex
)

add_custom_command(TARGET desktop-pets POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/conf
        $<TARGET_FILE_DIR:desktop-pets>/conf
)