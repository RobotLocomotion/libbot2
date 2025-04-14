# Default CPack generators
set(CPACK_GENERATOR TGZ STGZ)

# Detect OS type, OS variant and target architecture
if(UNIX)
  if(APPLE)
    set(OS_TYPE macos)
  else()
    set(OS_TYPE linux)
    # Determine distribution
    file(READ /etc/lsb-release lsb_release)
    string(REGEX REPLACE "^((.*ID|NAME)=)([a-zA-Z0-9]*).*" "\\3"  LINUX_DISTRO ${lsb_release})
  endif()
  # Determine architecture
  if(LINUX_DISTRO STREQUAL "Debian" OR LINUX_DISTRO STREQUAL "Ubuntu" OR LINUX_DISTRO STREQUAL "LinuxMint")
    execute_process(COMMAND dpkg --print-architecture
      OUTPUT_VARIABLE MACHINE_ARCH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(OS_TYPE_ARCH_SUFFIX ${MACHINE_ARCH})
  else()
    execute_process(COMMAND uname -m
        OUTPUT_VARIABLE MACHINE_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # Set OS type and ARCH suffix
    set(OS_TYPE_ARCH_SUFFIX ${OS_TYPE}-${MACHINE_ARCH})
  endif()
elseif(WIN32)
  set(OS_TYPE win)
  # Determine architecture
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(MACHINE_ARCH 64)
  else()
    set(MACHINE_ARCH "")
  endif()
  # Set OS type and ARCH suffix
  set(OS_TYPE_ARCH_SUFFIX ${OS_TYPE}${MACHINE_ARCH})
endif()

include(InstallRequiredSystemLibraries)

# Package release version
set(PACKAGE_RELEASE_VERSION 1)
set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};.;libbot2;ALL;/")

set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/packages)
# Caveat: CMAKE_INSTALL_PREFIX and CPACK_PACKAGING_INSTALL_PREFIX have to match because python
# scripts generated at compile-time hardcode paths.
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CPACK_PACKAGE_VENDOR "Kitware, Inc.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_CHECKSUM SHA256)
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/RobotLocomotion/libbot2")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}-${PACKAGE_RELEASE_VERSION}_${OS_TYPE_ARCH_SUFFIX}")
set(CPACK_STRIP_FILES ON)
set(CPACK_SOURCE_STRIP_FILES OFF)

# Debian specific
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${MACHINE_ARCH})
set(CPACK_DEBIAN_PACKAGE_RELEASE ${PACKAGE_RELEASE_VERSION})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "lcm (>=1.4.0), default-jre | java8-runtime, freeglut3, libc6, libgcc1, libglib2.0-0, libglu1-mesa, libgtk-3-0, libice6, libjpeg8, libopengl0, libpng16-16, libsm6, libstdc++6, libx11-6, libxau6, libxcb1, libxdmcp6, libxext6, libxmu6, libxt6, python3, python3-gi, python3-numpy, python3-scipy, zlib1g")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${CPACK_PACKAGE_HOMEPAGE_URL}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR} <debian@kitware.com>")

include(CPack)
