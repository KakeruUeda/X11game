cmake_minimum_required(VERSION 3.10)
project(X11game-project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(X11)

message(STATUS "X11 include dirs: ${X11_INCLUDE_DIR}")
message(STATUS "X11 libraries: ${X11_LIBRARIES}")

add_executable(X11game main.cpp)

target_include_directories(X11game
PRIVATE 
    ${X11_INCLUDE_DIR}
    )

target_link_libraries(X11game
    ${X11_LIBRARIES}
    )