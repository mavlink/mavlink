# copy sources and create target with NAME to track copied files
function(GLOB_COPY_TRACKED)

    # arguments
    set(options RECURSE)
    set(onveValueArgs TARGET SRCDIR DESTDIR)
    set(multiValueArgs GLOB_ARGS)

    # parse
    cmake_parse_arguments(GLOB_COPY_TRACKED "${options}" "${onveValueArgs}" "${multiValueArgs}" ${ARGN})
    if (NOT "${GLOB_COPY_TRACKED_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "unknown arguments: ${GLOB_COPY_TRACKED_UNPARSED_ARGUMENTS}")
    endif()
    set(REQUIRED_ARGS
        TARGET
        DESTDIR
        GLOB_ARGS)
    foreach(ARG ${REQUIRED_ARGS})
        if ("GLOB_COPY_TRACKED_${ARG}" STREQUAL "")
            message(FATAL_ERROR "${ARG} argument required")
        endif()
    endforeach()

    if (GLOB_COPY_TRACKED_RECURSE)
        set(RECURSE_STRING "_RECURSE")
    else()
        set(RECURSE_STRING "")
    endif()

    file(GLOB${RECURSE_STRING} SRCS RELATIVE "${GLOB_COPY_TRACKED_SRCDIR}" "${GLOB_COPY_TRACKED_GLOB_ARGS}")
    set(${GLOB_COPY_TRACKED_TARGET}_COPIED_SRCS)
    #message(STATUS "sources: ${SRCS}")
    foreach(SRC ${SRCS})  
        add_custom_command(OUTPUT "${GLOB_COPY_TRACKED_DESTDIR}/${SRC}"
            COMMAND "${CMAKE_COMMAND}" -E copy "${GLOB_COPY_TRACKED_SRCDIR}/${SRC}" 
            "${GLOB_COPY_TRACKED_DESTDIR}/${SRC}" 
            DEPENDS "${GLOB_COPY_TRACKED_SRCDIR}/${SRC}")
        list(APPEND ${GLOB_COPY_TRACKED_TARGET}_COPIED_SRCS "${GLOB_COPY_TRACKED_DESTDIR}/${SRC}")
    endforeach()
    add_custom_target(${GLOB_COPY_TRACKED_TARGET}
       DEPENDS ${${GLOB_COPY_TRACKED_TARGET}_COPIED_SRCS}
       )
endfunction()
