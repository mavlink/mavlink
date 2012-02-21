project (mavlink NONE)

# settings
cmake_minimum_required (VERSION 2.8.2)
set(APPLICATION_NAME ${PROJECT_NAME})
set(PROJECT_CONTACT_VENDOR mavlink)
set(APPLICATION_VERSION_MAJOR "1")
set(APPLICATION_VERSION_MINOR "0")
set(APPLICATION_VERSION_PATCH "0")
set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")
set(LIBRARY_SOVERSION "0.0.0")
set(PROJECT_CONTACT_EMAIL http://groups.google.com/group/mavlink)
set(PROJECT_CONTACT_VENDOR mavlink)

# modules
set(CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake
	${CMAKE_SOURCE_DIR}/cmake/arkcmake
)

# use common setup
include(CommonSetup)
set(LIBRARY_VERSION ${APPLICATION_VERSION})

# modules
#include(CTest)

# cpack
include(CPackPackage)

# find packages
find_package(PythonInterp)

# settings
option(BUILD_TEST "Build test programs." OFF)

# enable languages
if (BUILD_TEST)
    enable_language(C)
    enable_language(CXX)
    include(DefineCompilerFlags)
endif()

# mavlink generation
macro(generateMavlink version definitions)
    foreach(definition ${definitions})
        set(targetName ${definition}-v${version})
        set(definitionAbsPath ${CMAKE_SOURCE_DIR}/message_definitions/v${version}/${definition})
        message(STATUS "processing: ${definitionAbsPath}")
        add_custom_command( 
            OUTPUT ${targetName}-stamp
            COMMAND ${PYTHON_EXECUTABLE} ${mavgen} --lang=C --wire-protocol=${version}
                --output=include/v${version} ${definitionAbsPath}
            COMMAND touch ${targetName}-stamp
            DEPENDS ${definitionAbsPath} ${mavgen}
            )
        add_custom_target(${targetName} ALL DEPENDS ${targetName}-stamp)
    endforeach()
endmacro()

# build
set(mavgen ${CMAKE_CURRENT_SOURCE_DIR}/pymavlink/generator/mavgen.py)
set(v0.9Definitions
    ardupilotmega.xml
    common.xml
    minimal.xml
    pixhawk.xml
    slugs.xml
    test.xml
    ualberta.xml
    )
generateMavlink("0.9" "${v0.9Definitions}")
set(v1.0Definitions
    ardupilotmega.xml
    common.xml
    matrixpilot.xml
    minimal.xml
    pixhawk.xml
    slugs.xml
    test.xml
    ualberta.xml
    )
generateMavlink("1.0" "${v1.0Definitions}")

# testing
if (BUILD_TEST)
    if (UNIX) 
        include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/v0.9/common)
        # TODO fix udp example
        #add_executable(mavlink_udp examples/linux/mavlink_udp.c)
    endif()
endif()

# install files
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/ DESTINATION include/${PROJECT_NAME} FILES_MATCHING PATTERN "*.h")

# pkgconfig
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc)
install(FILES
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION lib/pkgconfig
    )

# vim:sw=4:ts=4:expandtab
