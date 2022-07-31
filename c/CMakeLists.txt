cmake_minimum_required(VERSION 3.12...3.22)

project(
    "Management Systems in C"
    LANGUAGES "C"
    VERSION "0.1.0"
)

configure_file("config.h.cin" "${PROJECT_BINARY_DIR}/config.h")

find_package(CapnProto REQUIRED)

set(INCDIR "${PROJECT_BINARY_DIR}/include")
add_custom_target(
    create_incdir COMMAND "${CMAKE_COMMAND}" -E make_directory "${INCDIR}"
)
add_custom_command(
    OUTPUT "${INCDIR}/bank.capnp.c" "${INCDIR}/bank.capnp.h"
    DEPENDS create_incdir "${PROJECT_SOURCE_DIR}/bank/proto/bank.capnp"
    COMMAND
        "${CAPNP_EXECUTABLE}" compile -o "c:${INCDIR}" --src-prefix
        "${PROJECT_SOURCE_DIR}/bank/proto"
        "${PROJECT_SOURCE_DIR}/bank/proto/bank.capnp"
)
pushd_executable("bank" "bank.c" "ser.c" "string.c")
target_sources("bank" PRIVATE "${INCDIR}/bank.capnp.c")
target_include_directories(
    "bank" PRIVATE "bank/include" "${PROJECT_BINARY_DIR}/include"
)
target_link_libraries("bank" PRIVATE capnp_c)
