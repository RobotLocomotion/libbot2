cmake_minimum_required(VERSION 3.10)
project(bot2-param LANGUAGES C CXX VERSION 0.0.1)

set(JAVA_INSTALL_SUBDIR share/java) # java
macro(configure_import_targets _prop)
  if("${_prop}" STREQUAL "BUILD")
    # BUILD tree configure
    set(JAVA_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
    configure_file(cmake/${PROJECT_NAME}-config.cmake.in
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" @ONLY)
  elseif("${_prop}" STREQUAL "INSTALL")
    # INSTALL tree configure
    set(JAVA_INSTALL_DIR \${\${CMAKE_FIND_PACKAGE_NAME}_IMPORT_PREFIX}/${JAVA_INSTALL_SUBDIR})
    configure_file(cmake/${PROJECT_NAME}-config.cmake.in
        "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake" @ONLY)
  else()
    message(FATAL_ERROR "Choose between BUILD and INSTALL")
  endif()
endmacro()

set(POD_NAME ${PROJECT_NAME})
set(NAMESPACE libbot2::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)
if(NOT BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(cmake/pods.cmake)
pods_config_search_paths()

# automatically build LCM types.  This also defines a number of CMake
# variables, see cmake/lcmtypes.cmake for details
include(cmake/lcmtypes.cmake)
lcmtypes_build(EXPORT ${PROJECT_NAME})

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")
find_package(GLib2 MODULE REQUIRED)

set(EXPORT_FILE ${PROJECT_NAME}-targets.cmake)

configure_import_targets(BUILD)
configure_import_targets(INSTALL)

add_subdirectory(src/param_client)
add_subdirectory(src/param_server)
add_subdirectory(src/param_tester)

# Exported targets for build directory
export(EXPORT ${PROJECT_NAME}
  NAMESPACE ${NAMESPACE}
  FILE ${PROJECT_BINARY_DIR}/${EXPORT_FILE}
)

pods_install_cmake_config_files(NAMESPACE ${NAMESPACE}
                                EXPORT ${PROJECT_NAME}
                                EXPORT_FILE ${EXPORT_FILE}
                                DESTINATION lib/cmake/${PROJECT_NAME}
                                CONFIG cmake/${PROJECT_NAME}-config.cmake)
