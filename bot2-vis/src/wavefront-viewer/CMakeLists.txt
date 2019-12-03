add_executable(bot-wavefront-viewer
    main.c
    renderer_wavefront.c)
target_link_libraries(bot-wavefront-viewer bot2-vis ${GTK3_LIBRARIES} OpenGL::GL ${GLUT_LIBRARIES})
set_property(TARGET bot-wavefront-viewer PROPERTY C_STANDARD 99)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-wavefront-viewer)
