cmake_minimum_required(VERSION 3.28)
project(ascii-hangman LANGUAGES C)

find_package(Curses REQUIRED)

file(GLOB_RECURSE source_files CONFIGURE_DEPENDS src/*.c)
file(GLOB_RECURSE header_files CONFIGURE_DEPENDS include/*.h)

configure_file(res/wordlist-en ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(res/wordlist-de ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(${PROJECT_NAME} ${source_files} ${header_files})
target_compile_features(${PROJECT_NAME} PRIVATE c_std_23)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARY})
