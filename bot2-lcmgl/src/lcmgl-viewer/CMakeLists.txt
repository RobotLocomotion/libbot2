find_package(bot2-vis CONFIG REQUIRED)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig MODULE REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(bot-lcmgl-viewer
    main.c
    udp_util.c
    view_menu.c
    )
target_link_libraries(bot-lcmgl-viewer bot2-lcmgl-renderer libbot2::bot2-vis libbot2::bot2-core ${GTK3_LIBRARIES})
target_include_directories(bot-lcmgl-viewer PUBLIC ${GTK3_INCLUDE_DIRS})
set_property(TARGET bot-lcmgl-viewer PROPERTY C_STANDARD 99)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-lcmgl-viewer)
