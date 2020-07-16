cmake_minimum_required(VERSION 3.15)

project(PreferansSolver LANGUAGES CXX)

SET(SOURCE_FILES
	CardDefs.h
	CardPack.cpp
	CardPack.h
	Game.cpp
	Game.h
	Player.cpp
	Player.h
	Score.cpp
	Score.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

