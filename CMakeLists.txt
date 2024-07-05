cmake_minimum_required(VERSION 3.28)
project(ascii-hangman LANGUAGES C)

find_package(Curses REQUIRED)

set(SOURCES
        src/main.c
        src/game.c
        src/hangman.c
        src/menu.c
        src/sprites.c
)

set(INCLUDE_DIRS ./include ${CURSES_INCLUDE_DIR})

configure_file(res/wordlist-en ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(res/wordlist-de ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CURSES_LIBRARY})
target_compile_features(${PROJECT_NAME} PUBLIC c_std_11)
