cmake_minimum_required(VERSION 3.13)

project(mavlink)

find_package(Python COMPONENTS Interpreter REQUIRED)

# We automatically install the pip dependencies locally below.
# Therefore, we check whether pip is available here.
execute_process(
    COMMAND ${Python_EXECUTABLE} -m pip -V
    RESULT_VARIABLE EXIT_CODE
    OUTPUT_QUIET
)
if (NOT ${EXIT_CODE} EQUAL 0)
    message(FATAL_ERROR "Python pip not found, pip is required")
endif()

if (NOT MAVLINK_DIALECT)
    set(MAVLINK_DIALECT common)
endif()
message(STATUS "MAVLink dialect: ${MAVLINK_DIALECT}")

if (NOT MAVLINK_VERSION)
    set(MAVLINK_VERSION 2.0)
endif()
message(STATUS "MAVLink version: ${MAVLINK_VERSION}")

set(EXAMPLE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/include/mavlink/${MAVLINK_DIALECT}/mavlink.h)

add_custom_command(OUTPUT ${EXAMPLE_HEADER}
    COMMAND ${Python_EXECUTABLE}
        -m pip install -r pymavlink/requirements.txt --upgrade -t ${CMAKE_CURRENT_BINARY_DIR}/pip-dependencies/
        COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}/pip-dependencies/" ${Python_EXECUTABLE}
        -m pymavlink.tools.mavgen
        --lang=C
        --wire-protocol=${MAVLINK_VERSION}
        --output ${CMAKE_CURRENT_BINARY_DIR}/include/mavlink/
        message_definitions/v1.0/${MAVLINK_DIALECT}.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS message_definitions/v1.0/${MAVLINK_DIALECT}.xml
    COMMENT "Generating C headers")

# Unfortunately, the dependencies don't work for INTERFACE libraries.
# The only way I could make it work is to add ALL which means it
# will do the file generation every time even when nothing has changed.
add_custom_target(generate_c_headers
    ALL
    DEPENDS ${EXAMPLE_HEADER})

add_library(mavlink INTERFACE)

add_dependencies(mavlink generate_c_headers)

include(GNUInstallDirs)

target_include_directories(mavlink
    INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS mavlink
    EXPORT MAVLinkTargets
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/mavlink"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT MAVLinkTargets
    FILE MAVLinkTargets.cmake
    NAMESPACE MAVLink::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MAVLink
)

# For the build tree
configure_file(MAVLinkConfig.cmake.in
    "${PROJECT_BINARY_DIR}/MAVLinkConfig.cmake" @ONLY)
# For the install tree
configure_file(MAVLinkConfig.cmake.in
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/MAVLinkConfig.cmake" @ONLY)

install(FILES
    "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/MAVLinkConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MAVLink
)
