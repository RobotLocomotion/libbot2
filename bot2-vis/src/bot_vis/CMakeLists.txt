file(GLOB c_files *.c)
file(GLOB h_files *.h)

# Keep the GLM library out of the public API
list(REMOVE_ITEM h_files glm.h)

find_package(bot2-core CONFIG REQUIRED)
find_package(X11 MODULE REQUIRED)
find_package(ZLIB MODULE REQUIRED)
find_package(JPEG MODULE REQUIRED)
find_package(PNG MODULE REQUIRED)

set(REQUIRED_LIBS ${JPEG_LIBRARY} -lglut -lz)

if(APPLE)
  set(REQUIRED_LIBS ${REQUIRED_LIBS} ${X11_LIBRARIES})
endif(APPLE)

add_library(bot2-vis ${c_files})
target_link_libraries(bot2-vis
  PUBLIC
    libbot2::lcmtypes_bot2-core
    libbot2::bot2-core
    GLib2::glib
    OpenGL::GL
    OpenGL::GLU
    ${GLUT_LIBRARIES}
    ${GTK3_LIBRARIES}
    m
  PRIVATE
    pthread
    ${X11_LIBRARIES}
    ${PNG_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${ZLIB_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
    ${GTK3_INCLUDE_DIRS}
  PRIVATE
    ${X11_INCLUDE_DIR}
    ${PNG_INCLUDE_DIRS}
    ${JPEG_INCLUDE_DIR}
)
set_property(TARGET bot2-vis PROPERTY C_STANDARD 99)
target_compile_options(bot2-vis PRIVATE ${PNG_DEFINITIONS})
set(REQUIRED_PACKAGES glib-2.0 gtk+-3.0 gdk-pixbuf-3.0 lcm bot2-core libpng gl glu gthread-2.0)

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(bot2-vis PROPERTIES SOVERSION 1)

pods_install_libraries(EXPORT ${PROJECT_NAME} ${PROJECT_NAME})

pods_install_headers(${h_files} DESTINATION bot_vis)

pods_install_pkg_config_file(${PROJECT_NAME}
    LIBS -lbot2-vis ${REQUIRED_LIBS}
    REQUIRES ${REQUIRED_PACKAGES}
    VERSION 0.0.1)
