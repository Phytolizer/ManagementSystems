cmake_minimum_required(VERSION 3.12...3.22)

project(
    "Management Systems in C"
    LANGUAGES "C"
    VERSION "0.1.0"
)

configure_file("config.h.cin" "${PROJECT_BINARY_DIR}/config.h")

add_subdirectory("third/binn")

pushd_executable("bank" "bank.c")
target_include_directories(bank PRIVATE "${PROJECT_BINARY_DIR}")
target_link_libraries("bank" PRIVATE binn)
