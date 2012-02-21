# application name
if (NOT APPLICATION_NAME)
    message(STATUS "${PROJECT_NAME} is used as APPLICATION_NAME")
    set(APPLICATION_NAME ${PROJECT_NAME} INTERNAL)
endif (NOT APPLICATION_NAME)

# library settings
set(INSTALL_LIB_SUFFIX CACHE STRING "Define suffix of directory name (32/64)")

if (NOT CPACK_INSTALL_PREFIX) 
    set(CPACK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()

# install prefixes
if (APPLE AND (CPACK_GENERATOR STREQUAL "Bundle"))
	set(CPACK_GENERATOR_PREFIX "/Applications/${APPLICATION_NAME}.app/")
elseif(WIN32)
	set(CPACK_GENERATOR_PREFIX "C:/Program Files/${APPLICATION_NAME}-${APPLICATION_VERSION}/")
else()
	set(CPACK_GENERATOR_PREFIX "")
endif()
set(INSTALL_EXEC_PREFIX "${CPACK_GENERATOR_PREFIX}/${CPACK_INSTALL_PREFIX}" CACHE PATH  "Base directory for executables and libraries")
set(INSTALL_SHARE_PREFIX "${CPACK_GENERATOR_PREFIX}/${CPACK_INSTALL_PREFIX}/share" CACHE PATH "Base directory for files which go to share/")
set(INSTALL_DATA_PREFIX "${INSTALL_SHARE_PREFIX}/${APPLICATION_NAME}" CACHE PATH "The parent directory where applications can install their data")

# The following are directories where stuff will be installed to
set(INSTALL_BIN_DIR "${INSTALL_EXEC_PREFIX}/bin" CACHE PATH "The ${APPLICATION_NAME} binary install dir (default prefix/bin)")
set(INSTALL_SBIN_DIR "${INSTALL_EXEC_PREFIX}/sbin" CACHE PATH "The ${APPLICATION_NAME} sbin install dir (default prefix/sbin)")
set(INSTALL_LIB_DIR "${INSTALL_EXEC_PREFIX}/lib${LIB_SUFFIX}" CACHE PATH "The subdirectory relative to the install prefix where libraries will be installed (default is prefix/lib)")
set(INSTALL_LIBEXEC_DIR "${INSTALL_EXEC_PREFIX}/libexec" CACHE PATH "The subdirectory relative to the install prefix where libraries will be installed (default is prefix/libexec)")
set(INSTALL_PLUGIN_DIR "${INSTALL_LIB_DIR}/${APPLICATION_NAME}" CACHE PATH "The subdirectory relative to the install prefix where plugins will be installed (default is prefix/lib/${APPLICATION_NAME})")
set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "The subdirectory to the header prefix (default prefix/include)")
set(INSTALL_DATA_DIR "${INSTALL_DATA_PREFIX}" CACHE PATH "The parent directory where applications can install their data (default prefix/share/${APPLICATION_NAME})")
set(INSTALL_HTML_DIR "${INSTALL_DATA_PREFIX}/doc/HTML" CACHE PATH "The HTML install dir for documentation (default data/doc/html)")
set(INSTALL_ICON_DIR "${INSTALL_DATA_PREFIX}/icons" CACHE PATH "The icon install dir (default data/icons/)")
set(INSTALL_SOUND_DIR "${INSTALL_DATA_PREFIX}/sounds" CACHE PATH "The install dir for sound files (default data/sounds)")
set(INSTALL_LOCALE_DIR "${INSTALL_SHARE_PREFIX}/locale" CACHE PATH "The install dir for translations (default prefix/share/locale)")
set(INSTALL_XDG_APPS_DIR "${INSTALL_SHARE_PREFIX}/applications/" CACHE PATH "The XDG apps dir")
set(INSTALL_XDG_DIRECTORY_DIR "${INSTALL_SHARE_PREFIX}/desktop-directories" CACHE PATH "The XDG directory")
set(INSTALL_SYSCONF_DIR "${INSTALL_EXEC_PREFIX}/etc" CACHE PATH "The ${APPLICATION_NAME} sysconfig install dir (default prefix/etc)")
set(INSTALL_MAN_DIR "${INSTALL_SHARE_PREFIX}/man" CACHE PATH "The ${APPLICATION_NAME} man install dir (default prefix/man)")
set(INSTALL_INFO_DIR "${INSTALL_SHARE_PREFIX}/info" CACHE PATH "The ${APPLICATION_NAME} info install dir (default prefix/info)")
