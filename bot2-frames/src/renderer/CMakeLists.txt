# Create a shared library libbot2-frames-renderer.so with a single source file
add_library(bot2-frames-renderers
    coord_frames_renderer.c articulated_body_renderer.c frame_modifier_renderer.c)
set_property(TARGET bot2-frames-renderers PROPERTY C_STANDARD 99)
target_link_libraries(bot2-frames-renderers
  PUBLIC
    bot2-frames
    libbot2::bot2-vis
  PRIVATE
    ${OPENGL_LIBRARIES}
    libbot2::bot2-core
    libbot2::bot2-param-client)
target_include_directories(bot2-frames-renderers
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(bot2-frames-renderers PROPERTIES SOVERSION 1)

# make the header public
# install it to include/bot_frames
pods_install_headers(bot_frames_renderers.h DESTINATION bot_frames)

# make the library public
pods_install_libraries(EXPORT ${PROJECT_NAME} bot2-frames-renderers)

# create a pkg-config file for the library, to make it for other software to
# use it.
set(REQUIRED_PACKAGES bot2-core bot2-param-client bot2-vis bot2-frames)
pods_install_pkg_config_file(bot2-frames-renderers
    CFLAGS
    LIBS -lbot2-frames-renderers
    REQUIRES ${REQUIRED_PACKAGES}
    VERSION 0.0.1)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig MODULE REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

find_package(GLUT MODULE REQUIRED)
find_package(bot2-lcmgl CONFIG REQUIRED)

#build the test-viewer
add_executable(test-viewer test_viewer.c)
target_link_libraries(test-viewer ${GLUT_LIBRARIES} bot2-frames-renderers libbot2::bot2-vis libbot2::bot2-lcmgl-renderer)
target_include_directories(test-viewer PRIVATE ${GLUT_INCLUDE_DIR} ${GTK3_INCLUDE_DIRS})
set_property(TARGET test-viewer PROPERTY C_STANDARD 99)
