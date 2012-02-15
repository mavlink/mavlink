project (mavlink NONE)

# settings
cmake_minimum_required (VERSION 2.8.2)
set(APPLICATION_NAME ${PROJECT_NAME})
set(PROJECT_CONTACT_VENDOR mavlink)
set(APPLICATION_VERSION_MAJOR "1")
set(APPLICATION_VERSION_MINOR "0")
set(APPLICATION_VERSION_PATCH "0")
#set(LIBRARY_SOVERSION "0.0.0")
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
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0501) # target xp
	set(CPACK_GENERATOR "NSIS")
	set(CPACK_SOURCE_GENERATOR "ZIP")
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    #set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
    #set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\mavlink.exe")
    set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}-${APPLICATION_VERSION}")
    set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\github.com/mavlink/mavlink")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\github.com/mavlink/mavlink")
    set(CPACK_NSIS_CONTACT ${PROJECT_CONTACT_EMAIL})
    set(CPACK_NSIS_MODIFY_PATH ON)
else()
    #set(CPACK_STRIP_FILES "bin/ins")
    #set(CPACK_SOURCE_STRIP_FILES "")
    if(APPLE)
        set(CPACK_GENERATOR "PackageMaker")
        set(CPACK_SOURCE_GENERATOR "TGZ")
            elseif(UNIX)
        set(CPACK_GENERATOR "DEB")
        set(CPACK_SOURCE_GENERATOR "ZIP")
    else()
        message(FATAL_ERROR "unknown operating system")
    endif()
endif()
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "micro air vehicle message marshalling library")
set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
set(CPACK_PACKAGE_CONTACT ${PROJECT_CONTACT_EMAIL})
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README")
set(CPACK_PACKAGE_VERSION_MAJOR "${APPLICATION_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${APPLICATION_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${APPLICATION_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}-${APPLICATION_VERSION}")
set(CPACK_SET_DESTDIR TRUE)
set(CPACK_SOURCE_IGNORE_FILES ${CPACK_SOURCE_IGNORE_FILES}
	/.git/;/build/;~$;.*\\\\.bin$;.*\\\\.swp$)
set(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
#set(CPACK_PACKAGE_EXECUTABLES "MyExecutable" "My Executable")
include(CPack)

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
        set(targetName ${definition}-${version})
        set(definitionAbsPath ${CMAKE_SOURCE_DIR}/message_definitions/${version}/${definition})
        message(STATUS "processing: ${definitionAbsPath}")
        add_custom_command( 
            OUTPUT ${targetName}-stamp
            COMMAND ${PYTHON_EXECUTABLE} ${mavgen} --lang=C --wire-protocol=0.9 
                --output=include/${version} ${definitionAbsPath}
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
generateMavlink("v0.9" "${v0.9Definitions}")
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
generateMavlink("v1.0" "${v1.0Definitions}")

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
