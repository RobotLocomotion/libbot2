add_library(bot2-param-client
    param_internal.c param_util.c)
    
find_package(bot2-core CONFIG REQUIRED)
target_link_libraries(bot2-param-client PUBLIC ${LCM_NAMESPACE}lcm libbot2::bot2-core libbot2::lcmtypes_bot2-core PRIVATE GLib2::glib lcmtypes_bot2-param)
target_include_directories(bot2-param-client PUBLIC
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include> )
set_property(TARGET bot2-param-client PROPERTY C_STANDARD 99)

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(bot2-param-client PROPERTIES SOVERSION 1)

# make the header public
pods_install_headers(param_client.h param_util.h DESTINATION bot_param)

# make the library public
pods_install_libraries(EXPORT ${PROJECT_NAME} bot2-param-client)

# create a pkg-config file for the library, to make it for other software to
# use it.
set(REQUIRED_LIBS lcm bot2-core glib-2.0 gthread-2.0 lcmtypes_bot2-param)
pods_install_pkg_config_file(bot2-param-client
    LIBS -lbot2-param-client
    REQUIRES ${REQUIRED_LIBS}
    VERSION 0.0.1)
