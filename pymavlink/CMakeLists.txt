# pymavlink support's python2 only
set(PythonInterp_FIND_VERSION "2")
find_package(PythonInterp REQUIRED)

# part of catkin/python.cmake
set(enable_setuptools_deb_layout OFF)
if(EXISTS "/etc/debian_version")
  set(enable_setuptools_deb_layout ON)
endif()
option(SETUPTOOLS_DEB_LAYOUT "Enable debian style python package layout" ${enable_setuptools_deb_layout})

if(SETUPTOOLS_DEB_LAYOUT)
  message(STATUS "Using Debian Python package layout")
  set(SETUPTOOLS_ARG_EXTRA "--install-layout=deb")
  set(PYTHON_PACKAGES_DIR dist-packages)
else()
  message(STATUS "Using default Python package layout")
  set(PYTHON_PACKAGES_DIR site-packages)
endif()

# linux
set(_PYTHON_PATH_VERSION_SUFFIX ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})
set(PYTHON_INSTALL_DIR lib/python${_PYTHON_PATH_VERSION_SUFFIX}/${PYTHON_PACKAGES_DIR}
  CACHE INTERNAL "For 'setup.py install'")

# prepare installation script
set(INSTALL_CMD_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(INSTALL_SCRIPT_TMP ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/python_distutils_install.sh)
set(INSTALL_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/python_distutils_install.sh)
configure_file(python_distutils_install.sh.in ${INSTALL_SCRIPT_TMP} @ONLY)

# workaround for setting exec perms on install script
file(COPY ${INSTALL_SCRIPT_TMP} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

set(INSTALL_CMAKE ${CMAKE_CURRENT_BINARY_DIR}/safe_execute_install.cmake)
configure_file(safe_execute_install.cmake.in ${INSTALL_CMAKE} @ONLY)

install(SCRIPT ${INSTALL_CMAKE})
