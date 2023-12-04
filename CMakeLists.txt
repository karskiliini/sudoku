
cmake_minimum_required(VERSION 3.23)

project(karski_sudoku)
set(CMAKE_CXX_STANDARD 17)


# library
add_library(solver
    src/main.cpp
    src/board.cpp
    src/ninth.cpp
    src/cell.cpp
    src/strategy.cpp
)

target_include_directories(solver
    PUBLIC inc
)

# executable
add_executable(solverApp
    src/main.cpp
    src/board.cpp
    src/ninth.cpp
    src/cell.cpp
    src/strategy.cpp
)

target_include_directories(solverApp
    PRIVATE inc
)


enable_testing()
add_subdirectory(test)
