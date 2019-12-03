cmake_minimum_required(VERSION 3.10)
project(bot2-vis LANGUAGES C CXX VERSION 0.0.1)

set(POD_NAME ${PROJECT_NAME})
set(NAMESPACE libbot2::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)
if(NOT BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")
include(cmake/pods.cmake)

find_package(GLib2 MODULE REQUIRED)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig MODULE REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

find_package(lcm 1.4 CONFIG REQUIRED)
find_package(OpenGL MODULE REQUIRED)
find_package(GLUT MODULE REQUIRED)

add_subdirectory(src/bot_vis)
add_subdirectory(src/testers)
add_subdirectory(src/rwx-viewer)
add_subdirectory(src/wavefront-viewer)

# bot-ppmsgz script
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/bot-ppmsgz DESTINATION bin)

set(EXPORT_FILE ${PROJECT_NAME}-targets.cmake)
configure_file(cmake/${PROJECT_NAME}-config.cmake.in
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" @ONLY)
configure_file(cmake/modules/FindGLib2.cmake
    ${PROJECT_BINARY_DIR}/modules/FindGLib2.cmake COPYONLY)
configure_file(cmake/modules/FindOpenGL.cmake
    ${PROJECT_BINARY_DIR}/modules/FindOpenGL.cmake COPYONLY)

# Install FindGLib2.cmake which is not a standard CMake file
install(FILES cmake/modules/FindGLib2.cmake DESTINATION lib/cmake/${PROJECT_NAME}/modules)
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
                                CONFIG ${PROJECT_NAME}-config.cmake)
