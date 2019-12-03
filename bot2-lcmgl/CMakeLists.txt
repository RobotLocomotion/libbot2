cmake_minimum_required(VERSION 3.10)
project(bot2-lcmgl LANGUAGES C CXX VERSION 0.0.1)

set(JAVA_INSTALL_SUBDIR share/java) # java
macro(configure_import_targets _prop)
  if("${_prop}" STREQUAL "BUILD")
    # BUILD tree configure
    set(JAVA_BUILD_SUBDIR /java) # subdirectory for targets built in subdirectory "java"
    set(JAVA_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
    configure_file(cmake/${PROJECT_NAME}-config.cmake.in
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" @ONLY)
    unset(JAVA_BUILD_SUBDIR) # Not set for install tree configuration: All targets in same folder.
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

include(cmake/lcmtypes.cmake)
lcmtypes_build(JAVA_DEST_DIR ${PROJECT_SOURCE_DIR}/java/src
    EXPORT ${PROJECT_NAME})
find_package(bot2-core CONFIG REQUIRED)
find_package(lcm 1.4 CONFIG REQUIRED)
find_package(ZLIB MODULE REQUIRED)

option(USE_BOT_VIS "Build bot2-lcmgl with libbot2 dependency." ON)

add_subdirectory(src/bot_lcmgl_client)
add_subdirectory(src/bot_lcmgl_render)

if(USE_BOT_VIS)
  add_subdirectory(src/lcmgl-viewer)
endif()
add_subdirectory(java)
add_subdirectory(python)

set(EXPORT_FILE ${PROJECT_NAME}-targets.cmake)
configure_import_targets(BUILD)
configure_import_targets(INSTALL)

# Install FindOpenGL.cmake which does not import target
# in the CMake 3 style before CMake 3.8.
install(FILES cmake/modules/FindOpenGL.cmake DESTINATION lib/cmake/${PROJECT_NAME}/modules)

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
