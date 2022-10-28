cmake_minimum_required(VERSION 3.0.0)
project(SUDOKUS VERSION 0.0.1)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 14)

find_package(SFML REQUIRED COMPONENTS system window graphics)

set(SOURCES src/main.cpp 
            src/sudokus.cpp 
            src/utils.hpp
            src/states/game-state.cpp 
            src/states/splashscreen-state.cpp 
            src/states/playing-state.cpp 
            src/states/solving-state.cpp 
            src/grid.cpp
)
add_executable(SUDOKUS ${SOURCES})

target_link_libraries (SUDOKUS PRIVATE sfml-graphics sfml-window sfml-system)

include(CPack)
