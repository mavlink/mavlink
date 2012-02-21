#set(CPACK_PACKAGE_EXECUTABLES
    #"MyExecutable" "My Executable"
    #)

# generic settings
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "micro air vehicle message marshalling library")
set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
set(CPACK_PACKAGE_CONTACT ${PROJECT_CONTACT_EMAIL})
set(CPACK_PACKAGE_VERSION_MAJOR "${APPLICATION_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${APPLICATION_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${APPLICATION_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}-${APPLICATION_VERSION}")
set(CPACK_SET_DESTDIR TRUE)
set(CPACK_SOURCE_IGNORE_FILES ${CPACK_SOURCE_IGNORE_FILES}
	/.git/;/build/;~$;.*\\\\.bin$;.*\\\\.swp$)

# os specific packaging
if(WIN32)
    set(CPACK_INSTALL_PREFIX "")
    add_definitions(-D_WIN32_WINNT=0x0501) # target xp
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_SOURCE_GENERATOR "ZIP")
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backslashes.

    set(CPACK_NSIS_INSTALLED_ICON_NAME "mavlink")
    set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}-${APPLICATION_VERSION}")
    set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\github.com/mavlink/mavlink")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\github.com/mavlink/mavlink")
    set(CPACK_NSIS_CONTACT ${PROJECT_CONTACT_EMAIL})
    # ON means prompt to modify environmental variable
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
    set(CPACK_PACKAGE_ICON "")

elseif(APPLE)
    option(WITH_OSXBUNDLE "Build an app Bundle vs a pkg" ON)
    set(CPACK_SOURCE_GENERATOR "TGZ")

    # mac requires all files to have a file extension set
    configure_file("${CMAKE_SOURCE_DIR}/COPYING" "${CMAKE_BINARY_DIR}/COPYING.txt" COPYONLY)
    configure_file("${CMAKE_SOURCE_DIR}/README" "${CMAKE_BINARY_DIR}/README.txt" COPYONLY)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_BINARY_DIR}/COPYING.txt")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_BINARY_DIR}/README.txt")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_BINARY_DIR}/README.txt")

    if (WITH_OSXBUNDLE)
        set(CPACK_GENERATOR "Bundle")
        set(CPACK_BUNDLE_NAME "${PROJECT_NAME}")
        set(CPACK_INSTALL_PREFIX "${CPACK_BUNDLE_NAME}.app/Contents")
        set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${APPLICATION_VERSION}-Bundle-OSX")
        set(CPACK_PACKAGE_ICON "")
        set(MACOSX_BUNDLE_EXECUTABLE_NAME "mavsim")
        set(CPACK_BUNDLE_STARTUP_COMMAND "${CMAKE_SOURCE_DIR}/cmake/macFind.sh")
        # generate plist
        include(MacroConfigureMacOSXBundlePlist)
        set(CPACK_BUNDLE_ICON "")
        set(CPACK_COPYRIGHT "Copyright LGPL")
        ConfigureMacOSXBundlePlist(${PROJECT_NAME} "" ${CPACK_BUNDLE_ICON} ${APPLICATION_VERSION} ${CPACK_COPYRIGHT})
        configure_file("${CMAKE_SOURCE_DIR}/cmake/MacOSXBundleInfo.plist.in"
            "${CMAKE_BINARY_DIR}/MacOSXBundleInfo.plist")
        set(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/MacOSXBundleInfo.plist")
    else()
        set(CPACK_GENERATOR "PackageMaker")
        set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/cmake/WELCOME.txt")
    endif()
    
elseif(UNIX)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_SOURCE_GENERATOR "ZIP")

    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
else()
    message(FATAL_ERROR "unknown operating system")
endif()

include(CPack)
set(CMAKE_INSTALL_PREFIX "${CPACK_INSTALL_PREFIX}")
