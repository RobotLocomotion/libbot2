cmake_minimum_required(VERSION 3.10)
string(TIMESTAMP timestamp "%Y%m%d" UTC)
project(libbot2
  VERSION 0.0.1.${timestamp}
  DESCRIPTION "Libraries, tools, and algorithms designed to facilitate robotics research"
  LANGUAGES C CXX
)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE STRING
    "Install path prefix, prepended onto install directories" FORCE)
endif()

option(PACKAGE_LIBBOT2 "Set install prefix to package libbot2 on linux" OFF)
if(PACKAGE_LIBBOT2)
  set(CMAKE_INSTALL_PREFIX "/opt/${CMAKE_PROJECT_NAME}/${PROJECT_VERSION}" CACHE PATH "Install location for project" FORCE)
  set(INSTALL_CMD INSTALL_COMMAND :)
  mark_as_advanced(FORCE _build_dir)
else()
  # PODs out-of-source build logic
  if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" OR CMAKE_INSTALL_PREFIX STREQUAL "C:/Program Files/${PROJECT_NAME}")
    mark_as_advanced(CLEAR _build_dir)
    set(INSTALL_CMD)
    find_file(_build_dir build PATHS ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/.. ${PROJECT_SOURCE_DIR}/../.. ${PROJECT_SOURCE_DIR}/../../.. ${PROJECT_SOURCE_DIR}/../../../..)
    if(_build_dir)
        set(CMAKE_INSTALL_PREFIX "${_build_dir}" CACHE PATH "Install location for project" FORCE)
    else()
      execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/build)
      set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/build)
    endif()
  endif()
endif()
message(STATUS CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})

# set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")

find_package(lcm 1.4 CONFIG REQUIRED)
find_package(GLib2 MODULE REQUIRED)
find_package(PkgConfig MODULE REQUIRED)
find_package(OpenGL MODULE REQUIRED)
find_package(PythonInterp 3.6 MODULE REQUIRED)

# Look for the gdkx.h header to decide whether the local gtk3 install has
# X11 support or not.
find_file(GDKX_HEADER gdkx.h PATH_SUFFIXES gtk-3.0/gdk)

if(GDKX_HEADER)
  message(STATUS "gdkx.h found, system appears to have GTK+ X11 support")
else()
  message(STATUS "gdkx.h not found, system does not appear to have GTK+ X11 support")
endif()

mark_as_advanced(GDKX_HEADER)

include(CMakeDependentOption)

cmake_dependent_option(WITH_BOT_VIS "Build with bot2-vis" ON GDKX_HEADER OFF)

include(ExternalProject)

set(SubprojectBinary_dir
${CMAKE_SYSTEM_PREFIX_PATH}
${CMAKE_PREFIX_PATH}
${PROJECT_BINARY_DIR}/bot2-core
${PROJECT_BINARY_DIR}/bot2-procman
${PROJECT_BINARY_DIR}/bot2-lcm-utils
${PROJECT_BINARY_DIR}/bot2-param
${PROJECT_BINARY_DIR}/bot2-vis
${PROJECT_BINARY_DIR}/bot2-lcmgl
${PROJECT_BINARY_DIR}/bot2-frames
)
string(REPLACE ";" ":::" SubprojectBinary_dir "${SubprojectBinary_dir}")

# pass thru certain cmake variables.
set(default_cmake_args
  -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
  -DCMAKE_BUILD_WITH_INSTALL_RPATH:BOOL=${CMAKE_BUILD_WITH_INSTALL_RPATH}
  -DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=${CMAKE_INSTALL_RPATH_USE_LINK_PATH}


  "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
  "-DCMAKE_C_COMPILER_LAUNCHER:FILEPATH=${CMAKE_C_COMPILER_LAUNCHER}"
  "-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}"
  "-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}"
  "-DCMAKE_CXX_COMPILER_LAUNCHER:FILEPATH=${CMAKE_CXX_COMPILER_LAUNCHER}"
  "-DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}"
  "-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}"
  "-DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}"
  "-DCMAKE_INSTALL_NAME_DIR:STRING=${CMAKE_INSTALL_NAME_DIR}"
  "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
  "-DCMAKE_INSTALL_RPATH:STRING=${CMAKE_INSTALL_RPATH}"
  "-DCMAKE_PREFIX_PATH:PATH=${SubprojectBinary_dir}"
  "-DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}"
  -DCMAKE_SKIP_INSTALL_RPATH:BOOL=${CMAKE_SKIP_INSTALL_RPATH}
  -DCMAKE_SKIP_RPATH:BOOL=${CMAKE_SKIP_RPATH}
  "-DCMAKE_STATIC_LINKER_FLAGS:STRING=${CMAKE_STATIC_LINKER_FLAGS}"
  -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
  "-Dlcm_DIR:PATH=${lcm_DIR}"
  "-DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}"
  )



ExternalProject_Add(bot2-core
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-core
  BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-core
  LIST_SEPARATOR ":::"
  CMAKE_ARGS ${default_cmake_args}
  ${INSTALL_CMD}
  BUILD_ALWAYS 1)
set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-core;bot2core;Unspecified;/")

option(WITH_BOT_PROCMAN "Build with bot2-procman" ON)

if(WITH_BOT_PROCMAN)
  ExternalProject_Add(bot2-procman
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-procman
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-procman
    LIST_SEPARATOR ":::"
    CMAKE_ARGS ${default_cmake_args}
    "-DLCM_PYTHON_DIR_HINTS:PATH=${LCM_PYTHON_DIR_HINTS}"
    ${INSTALL_CMD}
    BUILD_ALWAYS 1)
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-procman;bot2-procman;ALL;/")
endif()

option(WITH_BOT_LCM_UTILS "Build with lcm-utils" ON)

if(WITH_BOT_LCM_UTILS)
  ExternalProject_Add(bot2-lcm-utils
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-lcm-utils
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-lcm-utils
    LIST_SEPARATOR ":::"
    CMAKE_ARGS ${default_cmake_args}
    "-DLCM_PYTHON_DIR_HINTS:PATH=${LCM_PYTHON_DIR_HINTS}"
    ${INSTALL_CMD}
    BUILD_ALWAYS 1)
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-lcm-utils;bot2-lcm-utils;ALL;/")
endif()

option(WITH_BOT_PARAM "Build with bot2-param" ON)

if(WITH_BOT_PARAM)
  ExternalProject_Add(bot2-param
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-param
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-param
    LIST_SEPARATOR ":::"
    CMAKE_ARGS ${default_cmake_args}
    ${INSTALL_CMD}
    DEPENDS bot2-core)
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-param;bot2-param;ALL;/")
endif()

option(WITH_BOT_LCMGL "Build with bot2-lcmgl" ON)

cmake_dependent_option(WITH_BOT_FRAMES "Build with bot2-frames, requires bot2-lcmgl and bot2-param" ON "WITH_BOT_LCMGL AND WITH_BOT_PARAM" OFF)

if(WITH_BOT_VIS)
  message(STATUS "Configuring with bot2-vis, modules requiring GTK+ X11 will be built.")

  ExternalProject_Add(bot2-vis
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-vis
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-vis
    CMAKE_ARGS ${default_cmake_args}
    LIST_SEPARATOR ":::"
    ${INSTALL_CMD}
    DEPENDS bot2-core)
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-vis;bot2-vis;ALL;/")

  if(WITH_BOT_LCMGL)
    ExternalProject_Add(bot2-lcmgl
      SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-lcmgl
      BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-lcmgl
      LIST_SEPARATOR ":::"
      CMAKE_ARGS ${default_cmake_args} -DUSE_BOT_VIS:BOOL=${WITH_BOT_VIS}
      ${INSTALL_CMD}
      DEPENDS bot2-core bot2-vis)
    set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-lcmgl;bot2-lcmgl;ALL;/")

    if(WITH_BOT_FRAMES AND WITH_BOT_PARAM)
      ExternalProject_Add(bot2-frames
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-frames
        BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-frames
        LIST_SEPARATOR ":::"
        CMAKE_ARGS ${default_cmake_args}
        ${INSTALL_CMD}
        DEPENDS bot2-core bot2-param bot2-vis bot2-lcmgl)
      set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-frames;bot2-frames;ALL;/")
    endif()
  endif()
else()
  if(WITH_BOT_LCMGL)
    message(STATUS "Configuring without bot2-vis, modules requiring GTK+ X11 will not be built.")
    find_package(ZLIB MODULE REQUIRED)

    ExternalProject_Add(bot2-lcmgl
      SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-lcmgl
      BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-lcmgl
      CMAKE_ARGS ${default_cmake_args} -DUSE_BOT_VIS:BOOL=${WITH_BOT_VIS}
      LIST_SEPARATOR ":::"
      ${INSTALL_CMD}
      DEPENDS bot2-core)
    set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-lcmgl;bot2-lcmgl;ALL;/")

    if(WITH_BOT_FRAMES AND WITH_BOT_PARAM)
      ExternalProject_Add(bot2-frames
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-frames
        BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-frames
        CMAKE_ARGS ${default_cmake_args}
        LIST_SEPARATOR ":::"
        ${INSTALL_CMD}
        DEPENDS bot2-core bot2-param bot2-lcmgl)
      set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-frames;bot2-frames;ALL;/")
    endif()
  endif()
endif()

configure_file(cmake/${CMAKE_PROJECT_NAME}-config.cmake.in
        "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake" DESTINATION lib/cmake/${CMAKE_PROJECT_NAME})

if(PACKAGE_LIBBOT2)
  include(cmake/cpack.cmake)
endif()
