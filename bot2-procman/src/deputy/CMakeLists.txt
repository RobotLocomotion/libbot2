list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")
find_package(GLib2 MODULE REQUIRED COMPONENTS gthread)
find_package(lcm 1.4 CONFIG REQUIRED)

add_executable(bot-procman-deputy
    lcm_util.c
    procinfo.c
    procman.c
    procman_deputy.c
    signal_pipe.c)
set_property(TARGET bot-procman-deputy PROPERTY C_STANDARD 99)
target_link_libraries(bot-procman-deputy ${LCM_NAMESPACE}lcm GLib2::gthread GLib2::glib util lcmtypes_bot2-procman)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-procman-deputy)
