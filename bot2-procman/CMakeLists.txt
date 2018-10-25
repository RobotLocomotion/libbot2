cmake_minimum_required(VERSION 3.3)

macro(configure_import_targets _prop)
  if("${_prop}" STREQUAL "BUILD")
    # BUILD tree configure
    set(JAVA_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
    set(RUNTIME_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/python)
    configure_file(cmake/${POD_NAME}-config.cmake.in
      "${PROJECT_BINARY_DIR}/${POD_NAME}-config.cmake" @ONLY)
  elseif("${_prop}" STREQUAL "INSTALL")
    # INSTALL tree configure
    set(INSTALL_DIR \${\${CMAKE_FIND_PACKAGE_NAME}_IMPORT_PREFIX})
    set(JAVA_INSTALL_DIR ${INSTALL_DIR}/share/java)
    set(RUNTIME_INSTALL_DIR ${INSTALL_DIR}/bin)
    configure_file(cmake/${POD_NAME}-config.cmake.in
        "${PROJECT_BINARY_DIR}/cmake/${POD_NAME}-config.cmake" @ONLY)
  else()
    message(FATAL_ERROR "Choose between BUILD and INSTALL")
  endif()
endmacro()

set(POD_NAME bot2-procman)
set(NAMESPACE libbot2::)

include(cmake/pods.cmake)

# automatically build LCM types.  This also defines a number of CMake
# variables, see cmake/lcmtypes.cmake for details
include(cmake/lcmtypes.cmake)
lcmtypes_build(EXPORT ${POD_NAME})


add_subdirectory(src/deputy)
add_subdirectory(python)

set(EXPORT_FILE ${POD_NAME}-targets.cmake)
configure_import_targets(BUILD)
configure_import_targets(INSTALL)

# Exported targets for build directory
export(EXPORT ${POD_NAME}
  NAMESPACE ${NAMESPACE}
  FILE ${PROJECT_BINARY_DIR}/${EXPORT_FILE}
)

pods_install_cmake_config_files(NAMESPACE ${NAMESPACE}
                                EXPORT ${POD_NAME}
                                EXPORT_FILE ${EXPORT_FILE}
                                DESTINATION lib/cmake/${POD_NAME}
                                CONFIG cmake/${POD_NAME}-config.cmake)
