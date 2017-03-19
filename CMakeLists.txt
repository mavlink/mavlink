project (mavlink NONE)

# Note: patched version for installation as ROS 3-rd party library
# Provides C-headers and C++11

# settings
cmake_minimum_required (VERSION 2.8.2)
set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "9")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_CONTACT_EMAIL http://groups.google.com/group/mavlink)
set(PROJECT_CONTACT_VENDOR mavlink)

include(GNUInstallDirs)

# hack from urdfdom: by default this would be 'lib/x86_64-linux-gnu'
set(CMAKE_INSTALL_LIBDIR lib)

# find libraries with cmake modules
set(PythonInterp_FIND_VERSION "2")
find_package(PythonInterp REQUIRED)

# Try to read package version from package.xml
if(EXISTS ${CMAKE_SOURCE_DIR}/package.xml)
    file(WRITE ${CMAKE_BINARY_DIR}/package_version.py
        "import re, sys\n"
        "from xml.etree import ElementTree as ET\n"
        "doc = ET.parse('${CMAKE_SOURCE_DIR}/package.xml')\n"
        "ver = doc.find('version').text\n"
        "if re.match('\\d+\\.\\d+\\.\\d+', ver):\n"
        "    sys.stdout.write(ver)\n"
        "else:\n"
        "    sys.stderr.write('Version format error.\\n')\n"
        "    sys.exit(1)\n"
        )

    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/package_version.py
        OUTPUT_VARIABLE XML_VERSION
        RESULT_VARIABLE XML_RESULT
        )

    string(STRIP "${XML_VERSION}" XML_VERSION)
    if(NOT ${XML_RESULT})
        set(PROJECT_VERSION ${XML_VERSION})
        message(STATUS "Package version: ${PROJECT_VERSION}")
    else()
        message(WARNING "Package version: package.xml parse error, default used: ${PROJECT_VERSION}")
    endif()
endif()

# config files
configure_file(config.h.in config.h)
install(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION include/${PROJECT_NAME} COMPONENT Dev)

# mavlink generation
set(mavgen_path ${CMAKE_SOURCE_DIR}/pymavlink/tools/mavgen.py)
set(common_xml_path ${CMAKE_SOURCE_DIR}/message_definitions/v1.0/common.xml)
macro(generateMavlink_v10 definitions)
    foreach(definitionAbsPath ${definitions})
        get_filename_component(definition ${definitionAbsPath} NAME_WE)
        message(STATUS "processing v1.0: ${definitionAbsPath}")
        add_custom_command(
            OUTPUT include/v1.0/${definition}/${definition}.h
            COMMAND /usr/bin/env PYTHONPATH="${CMAKE_SOURCE_DIR}:$ENV{PYTHONPATH}"
                ${PYTHON_EXECUTABLE} ${mavgen_path} --lang=C --wire-protocol=1.0
                --output=include/v1.0 ${definitionAbsPath}
            DEPENDS ${definitionAbsPath} ${common_xml_path} ${mavgen_path}
            )
        add_custom_target(${definition}.xml-v1.0
            ALL DEPENDS include/v1.0/${definition}/${definition}.h
            )
    endforeach()
endmacro()
macro(generateMavlink_v20 definitions)
    foreach(definitionAbsPath ${definitions})
        get_filename_component(definition ${definitionAbsPath} NAME_WE)
        message(STATUS "processing v2.0: ${definitionAbsPath}")

        add_custom_command(
            #OUTPUT include/v2.0/${definition}/${definition}.h
            OUTPUT ${definition}-v2.0-c.stamp
            COMMAND /usr/bin/env PYTHONPATH="${CMAKE_SOURCE_DIR}:$ENV{PYTHONPATH}"
                ${PYTHON_EXECUTABLE} ${mavgen_path} --lang=C --wire-protocol=2.0
                --output=include/v2.0 ${definitionAbsPath}
            COMMAND touch ${definition}-v2.0-c.stamp
            DEPENDS ${definitionAbsPath} ${common_xml_path} ${mavgen_path}
            )
        add_custom_command(
            #OUTPUT include/v2.0/${definition}/${definition}.hpp
            OUTPUT ${definition}-v2.0-cxx.stamp
            COMMAND /usr/bin/env PYTHONPATH="${CMAKE_SOURCE_DIR}:$ENV{PYTHONPATH}"
                ${PYTHON_EXECUTABLE} ${mavgen_path} --lang=C++11 --wire-protocol=2.0
                --output=include/v2.0 ${definitionAbsPath}
            COMMAND touch ${definition}-v2.0-cxx.stamp
            DEPENDS ${definitionAbsPath} ${common_xml_path} ${mavgen_path}
            )
        add_custom_target(${definition}.xml-v2.0
            ALL DEPENDS
                ${definition}-v2.0-c.stamp
                ${definition}-v2.0-cxx.stamp
                #include/v2.0/${definition}/${definition}.h
                #include/v2.0/${definition}/${definition}.hpp
            )
    endforeach()

    # common should be last
    foreach(definitionAbsPath ${definitions})
        get_filename_component(definition ${definitionAbsPath} NAME_WE)
        add_dependencies(common.xml-v2.0 ${definition}.xml-v2.0)
    endforeach()
endmacro()

# build v1.0
file(GLOB V10DEFINITIONS ${CMAKE_SOURCE_DIR}/message_definitions/v1.0/*.xml)

# do not generate testing dialects
foreach(definition minimal test python_array_test)
    list(REMOVE_ITEM V10DEFINITIONS "${CMAKE_SOURCE_DIR}/message_definitions/v1.0/${definition}.xml")
endforeach()
list(SORT V10DEFINITIONS)

generateMavlink_v10("${V10DEFINITIONS}")

# build v2.0 if it is supported. for now it uses same definitions as v1.0
if(EXISTS "${CMAKE_SOURCE_DIR}/pymavlink/generator/C/include_v2.0")
    set(V20DEFINITIONS "${V10DEFINITIONS}")
    generateMavlink_v20("${V20DEFINITIONS}")
endif()

# build pymavlink
# NOTE: pymavlink removed from mavlink package on @tridge request.
#       use `rosdep install python-pymavlink` to solve that dependency.
#add_subdirectory(pymavlink)

# install files
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION include/${PROJECT_NAME} COMPONENT Dev FILES_MATCHING PATTERN "*.h*")
install(DIRECTORY ${CMAKE_BINARY_DIR}/src/ DESTINATION share/${PROJECT_NAME} COMPONENT Dev FILES_MATCHING PATTERN "*.c*")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/share/${PROJECT_NAME} DESTINATION share COMPONENT Dev FILES_MATCHING PATTERN "*.c*")

# thanks for urdfdom project
set(PKG_NAME ${PROJECT_NAME})
set(PKG2_NAME "${PROJECT_NAME}2")
set(PKG_VERSION ${PROJECT_VERSION})
set(PKG_DESC "MAVLink micro air vehicle marshalling / communication library")
set(PKG_LIBRARIES )
set(PKG_DEPENDS )
set(PKG_MAVLINK_DEFINITIONS "${V10DEFINITIONS}")
set(PKG2_MAVLINK_DEFINITIONS "${V20DEFINITIONS}")
foreach(def ${V10DEFINITIONS})
    get_filename_component(dialect "${def}" NAME_WE)
    list(APPEND PKG_MAVLINK_DIALECTS ${dialect})
endforeach()
foreach(def ${V20DEFINITIONS})
    get_filename_component(dialect "${def}" NAME_WE)
    list(APPEND PKG2_MAVLINK_DIALECTS ${dialect})
endforeach()


configure_file(config.cmake.in ${PROJECT_NAME}-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake/ COMPONENT cmake)

configure_file(pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/ COMPONENT pkgconfig)

# add file extensions and set resource files
configure_file("COPYING" "COPYING.txt" COPYONLY)
install(FILES ${PROJECT_BINARY_DIR}/COPYING.txt
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/ COMPONENT license)

install(FILES ${CMAKE_SOURCE_DIR}/package.xml
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/ COMPONENT catkin)

# vim:sw=4:ts=4:expandtab
