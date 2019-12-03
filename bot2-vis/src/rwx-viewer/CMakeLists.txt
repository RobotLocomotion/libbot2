add_executable(bot-rwx-viewer
    main.c
    renderer_rwx.c)
target_link_libraries(bot-rwx-viewer bot2-vis ${GTK3_LIBRARIES} OpenGL::GL ${GLUT_LIBRARIES})
set_property(TARGET bot-rwx-viewer PROPERTY C_STANDARD 99)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-rwx-viewer)
