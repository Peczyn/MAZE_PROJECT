cmake_minimum_required(VERSION 3.27)
project(MAZE_PROJECT)

set(CMAKE_CXX_STANDARD 17)

add_executable(MAZE_PROJECT main.cpp
        header.h)
       #MazeGeneratorSolver.cpp
        #SFML.cpp)



find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(MAZE_PROJECT sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
