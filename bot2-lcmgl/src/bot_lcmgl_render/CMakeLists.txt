set(c_files lcmgl_decode.c)
set(h_files lcmgl_decode.h)
set(REQUIRED_LIBS)

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")
find_package(OpenGL MODULE REQUIRED)

if(USE_BOT_VIS)
  find_package(bot2-vis CONFIG REQUIRED)
  list(APPEND c_files lcmgl_bot_renderer.c)
  list(APPEND h_files lcmgl_bot_renderer.h)
  list(APPEND REQUIRED_LIBS bot2-vis gl)
endif()


add_library(bot2-lcmgl-renderer ${c_files})
set_property(TARGET bot2-lcmgl-renderer PROPERTY C_STANDARD 99)
target_link_libraries(bot2-lcmgl-renderer PRIVATE ${OPENGL_LIBRARIES} ${ZLIB_LIBRARIES} libbot2::bot2-core libbot2::lcmtypes_bot2-core lcmtypes_bot2-lcmgl)
if(USE_BOT_VIS)
  target_compile_definitions(bot2-lcmgl-renderer PRIVATE USE_BOT_VIS)
  target_link_libraries(bot2-lcmgl-renderer PUBLIC libbot2::bot2-vis)
endif()
target_include_directories(bot2-lcmgl-renderer
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(bot2-lcmgl-renderer PROPERTIES SOVERSION 1)

pods_install_libraries(EXPORT ${PROJECT_NAME} bot2-lcmgl-renderer)

pods_install_headers(${h_files} DESTINATION bot_lcmgl_render)

list(APPEND REQUIRED_LIBS lcmtypes_bot2-lcmgl)
pods_install_pkg_config_file(bot2-lcmgl-renderer
    LIBS -lbot2-lcmgl-renderer
    REQUIRES ${REQUIRED_LIBS}
    VERSION 0.0.1)
