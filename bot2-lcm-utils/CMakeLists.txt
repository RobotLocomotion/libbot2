cmake_minimum_required(VERSION 3.10)
project(bot2-lcm-utils LANGUAGES C CXX VERSION 0.0.1)

macro(configure_import_targets _prop)
  if("${_prop}" STREQUAL "BUILD")
    # BUILD tree configure
    set(RUNTIME_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
    configure_file(cmake/${PROJECT_NAME}-config.cmake.in
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" @ONLY)
  elseif("${_prop}" STREQUAL "INSTALL")
    # INSTALL tree configure
    set(RUNTIME_INSTALL_DIR \${\${CMAKE_FIND_PACKAGE_NAME}_IMPORT_PREFIX}/bin)
    configure_file(cmake/${PROJECT_NAME}-config.cmake.in
        "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake" @ONLY)
  else()
    message(FATAL_ERROR "Choose between BUILD and INSTALL")
  endif()
endmacro()

set(POD_NAME ${PROJECT_NAME})
set(NAMESPACE libbot2::)

include(cmake/pods.cmake)

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")

find_package(lcm 1.4 CONFIG REQUIRED)
find_package(GLib2 MODULE REQUIRED)

add_subdirectory(src/logfilter)
add_subdirectory(src/logsplice)
add_subdirectory(src/who)
add_subdirectory(src/tunnel)
add_subdirectory(python)
if(TARGET ${LCM_NAMESPACE}lcm)
  get_target_property(LCM_INCLUDE_DIRS ${LCM_NAMESPACE}lcm
    INTERFACE_INCLUDE_DIRECTORIES)
  foreach(LCM_INCLUDE_DIR ${LCM_INCLUDE_DIRS})
    get_filename_component(LCM_PREFIX "${LCM_INCLUDE_DIR}" DIRECTORY)
    list(APPEND LCM_SPY_HINT "${LCM_PREFIX}/lcm-java")
  endforeach()
endif()

find_program(LCM_SPY_EXECUTABLE NAMES lcm-spy HINTS ${LCM_SPY_HINT})

if(NOT LCM_SPY_EXECUTABLE)
  message(FATAL_ERROR "Could NOT find lcm-spy")
endif()

# bot-spy
set(script_name bot-spy)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${script_name}.sh.in
  ${CMAKE_CURRENT_BINARY_DIR}/${script_name}
  @ONLY)

# install it...
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${script_name} DESTINATION bin)

set(EXPORT_FILE ${PROJECT_NAME}-targets.cmake)
configure_import_targets(BUILD)
configure_import_targets(INSTALL)

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
