cmake_minimum_required(VERSION 3.12...3.22)

project("LeetCode in C" LANGUAGES "C" VERSION "0.1.0")

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
endfunction(pushd_executable)

pushd_library("management" "util.c")
target_include_directories("management" PUBLIC "management/include")

add_custom_command(
    OUTPUT "${PROJECT_BINARY_DIR}/bank/proto/BankState.pb-c.c"
    COMMAND "protoc-c" "--c_out=${PROJECT_BINARY_DIR}"
            "-I${PROJECT_SOURCE_DIR}" "bank/proto/BankState.proto"
)
pushd_executable("bank" "bank.c")
if(UNIX)
    set_target_properties("bank" PROPERTIES OUTPUT_NAME "bank.exe")
endif(UNIX)
target_sources(
    "bank" PRIVATE "${PROJECT_BINARY_DIR}/bank/proto/BankState.pb-c.c"
)
target_include_directories("bank" PRIVATE "${PROJECT_BINARY_DIR}")
target_link_libraries("bank" PRIVATE "management" "protobuf-c")
