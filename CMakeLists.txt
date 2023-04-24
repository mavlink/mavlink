cmake_minimum_required(VERSION 3.13)

project(mavlink)

add_library(mavlink INTERFACE)

include(GNUInstallDirs)

target_include_directories(mavlink
    INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS mavlink
    EXPORT MAVLinkTargets
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/mavlink"
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


