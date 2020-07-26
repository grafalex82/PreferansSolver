cmake_minimum_required(VERSION 3.15)

project(PreferansSolver LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


SET(SOURCE_FILES
        CardDefs.h
        CardPack.cpp
        CardPack.h
        GameState.cpp
        GameState.h
        Player.cpp
        Player.h
        Score.cpp
        Score.h

        Test.cpp
        TestRunner.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

