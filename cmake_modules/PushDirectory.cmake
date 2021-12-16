function(pushd_library TARGET_NAME)
    list(TRANSFORM "ARGN" PREPEND "${TARGET_NAME}/src/")
    add_library("${TARGET_NAME}" ${ARGN})
endfunction(pushd_library)

function(pushd_executable TARGET_NAME)
    list(TRANSFORM "ARGN" PREPEND "${TARGET_NAME}/src/")
    add_executable("${TARGET_NAME}" ${ARGN})
    if(UNIX)
        set_target_properties(
            "${TARGET_NAME}" PROPERTIES OUTPUT_NAME "${TARGET_NAME}.exe"
        )
    endif(UNIX)
endfunction(pushd_executable)