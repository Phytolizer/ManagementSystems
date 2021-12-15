cmake_minimum_required(VERSION 3.12...3.22)

project(
    "Management Systems in C"
    LANGUAGES "C"
    VERSION "0.1.0"
)

list(APPEND "CMAKE_MODULE_PATH" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_AUTHOR_EMAIL "me@phytolizer.dev")
set(CMAKE_AUTHOR_NAME "Kyle Coffey")
configure_file("config.h.cin" "${PROJECT_BINARY_DIR}/config.h")

function(pushd_library TARGET_NAME)
    list(TRANSFORM "ARGN" PREPEND "${TARGET_NAME}/src/")
    add_library("${TARGET_NAME}" ${ARGN})
endfunction(pushd_library)

function(pushd_executable TARGET_NAME)
    list(TRANSFORM "ARGN" PREPEND "${TARGET_NAME}/src/")
    add_executable("${TARGET_NAME}" ${ARGN})
    if(UNIX)
        set_target_properties("${TARGET_NAME}" PROPERTIES OUTPUT_NAME "${TARGET_NAME}.exe")
    endif(UNIX)
endfunction(pushd_executable)

function(attach_protobuf TARGET_NAME PROTOBUF_FILE_BASENAME)
    add_custom_command(
        OUTPUT
            "${PROJECT_BINARY_DIR}/${TARGET_NAME}/proto/${PROTOBUF_FILE_BASENAME}.pb-c.c"
        COMMAND
            "protoc-c" "--c_out=${PROJECT_BINARY_DIR}"
            "-I${PROJECT_SOURCE_DIR}"
            "${TARGET_NAME}/proto/${PROTOBUF_FILE_BASENAME}.proto"
    )
    target_sources(
        "${TARGET_NAME}"
        PRIVATE
            "${PROJECT_BINARY_DIR}/${TARGET_NAME}/proto/${PROTOBUF_FILE_BASENAME}.pb-c.c"
    )
    target_include_directories("${TARGET_NAME}" PRIVATE "${PROJECT_BINARY_DIR}")
    target_link_libraries("${TARGET_NAME}" PRIVATE "protobuf-c")
endfunction(attach_protobuf)

pushd_library("management" "util.c")
target_include_directories("management" PUBLIC "management/include")

pushd_executable(
    "bank"
    "bank.c"
    "find.c"
    "load.c"
    "save.c"
    "structure.c"
    "today.c"
)
target_link_libraries("bank" PRIVATE "management")
attach_protobuf("bank" "BankState")

pushd_executable("quiz" "quiz.c")
target_link_libraries("quiz" PRIVATE "management")
