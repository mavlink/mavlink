#------------------------------------------------------------------------------- 
# This macro will set all the variables necessary to have a "good" OS X Application 
# bundle. The variables are as follows: 
#  PROJECT_NAME - which can be taken from the ${PROJECT_NAME} variable is needed 
#  DEBUG_EXTENSION - The extension used to denote a debug built Application. Typically 
#   this is '_debug' 
#  ICON_FILE_PATH - The complete path to the bundle icon file 
#  VERSION_STRING - The version string that you wish to use for the bundle. For OS X 
#   this string is usually XXXX.YY.ZZ in type. Look at the Apple docs for more info 
#------------------------------------------------------------------------------- 
macro(ConfigureMacOSXBundlePlist PROJECT_NAME DEBUG_EXTENSION ICON_FILE_PATH VERSION_STRING COPYRIGHT) 
  # message(STATUS "ConfigureMacOSXBundlePlist for ${PROJECT_NAME} ") 
  IF(CMAKE_BUILD_TYPE MATCHES "Release") 
    SET(DBG_EXTENSION "") 
  else() 
    set(DBG_EXTENSION ${DEBUG_EXTENSION}) 
  endif() 
  get_filename_component(ICON_FILE_NAME "${ICON_FILE_PATH}" NAME) 
    
 #CFBundleGetInfoString 
 SET(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}${DBG_EXTENSION} Version ${VERSION_STRING}, ${COPYRIGHT}") 
 SET(MACOSX_BUNDLE_ICON_FILE ${ICON_FILE_NAME}) 
 SET(MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}${DBG_EXTENSION}") 
 #CFBundleLongVersionString 
 SET(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME}${DBG_EXTENSION} Version ${VERSION_STRING}") 
 SET(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}${DBG_EXTENSION}) 
 SET(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION_STRING}) 
 SET(MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION_STRING}) 
 SET(MACOSX_BUNDLE_COPYRIGHT ${COPYRIGHT}) 
  
 SET(${PROJECT_NAME}_PROJECT_SRCS ${${PROJECT_NAME}_PROJECT_SRCS} ${ICON_FILE_PATH}) 
 SET_SOURCE_FILES_PROPERTIES(${ICON_FILE_PATH} PROPERTIES 
                             MACOSX_PACKAGE_LOCATION Resources) 
                              
endmacro()