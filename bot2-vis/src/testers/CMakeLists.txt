set(testers 
    gl_drawing_area_tester
    gl_image_area_tester
    param_widget_tester)

foreach(tester ${testers})
    add_executable(${tester} "${tester}.c")
    target_link_libraries(${tester} bot2-vis GLib2::glib ${GTK3_LIBRARIES} OpenGL::GL OpenGL::GLU)
    set_property(TARGET ${tester} PROPERTY C_STANDARD 99)
endforeach()
