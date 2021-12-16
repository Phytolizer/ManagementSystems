cmake_minimum_required(VERSION 3.12...3.22)

project("Management Systems")

list(APPEND "CMAKE_MODULE_PATH" "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")
include(PushDirectory)

set(CMAKE_AUTHOR_EMAIL "me@phytolizer.dev")
set(CMAKE_AUTHOR_NAME "Kyle Coffey")

add_subdirectory("c")
add_subdirectory("cpp")
