find_package(Java MODULE)
if(JAVA_COMPILE STREQUAL JAVA_COMPILE-NOTFOUND OR
       JAVA_ARCHIVE STREQUAL JAVA_ARCHIVE-NOTFOUND OR
 JAVA_COMPILE STREQUAL Java_JAVAC_EXECUTABLE-NOTFOUND OR
 JAVA_ARCHIVE STREQUAL Java_JAR_EXECUTABLE-NOTFOUND)
    message(STATUS "Not building Java LCM-SPY plugins (Can't find Java)")
    return()
endif()

message(STATUS "Found java, building Java LCM-SPY plugins")
message(STATUS "JAVA_COMPILE: ${JAVA_COMPILE}")
message(STATUS "JAVA_ARCHIVE: ${JAVA_ARCHIVE}")
message(STATUS "Found java, building Java LCM-SPY plugins")

set(src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(jar_fname lcmspy_plugins_bot2.jar)

set(classpath ${src_dir}:${LCM_JAR_FILE}:${LCMTYPES_JAR})


# =============

# get a list of all .java files
file(GLOB_RECURSE java_files ${src_dir}/*.java)

# where do we build .class files to?
set(class_dir ${CMAKE_CURRENT_BINARY_DIR}/java-build)

# convert the list of .java filenames to a list of .class filenames
foreach(javafile ${java_files})
    string(REPLACE .java .class tmp_fname ${javafile})
    string(REPLACE ${src_dir} ${class_dir} tmp_fname ${tmp_fname})
    list(APPEND class_files ${tmp_fname})
endforeach()

# create the java build directory if needed
if(NOT IS_DIRECTORY ${class_dir})
    file(MAKE_DIRECTORY ${class_dir})
endif()

# add a rule to build the .class files from from the .java files
add_custom_command(OUTPUT ${class_files} COMMAND
    ${JAVA_COMPILE} -cp ${classpath} -d ${class_dir} ${java_files}
    DEPENDS ${java_files} ${LCMTYPES_JAR})

# add a rule to build a .jar file from the .class files
add_custom_command(OUTPUT ${jar_fname} COMMAND
    ${JAVA_ARCHIVE} cf ${jar_fname} -C ${class_dir} .
    DEPENDS ${class_files})
add_custom_target(jar ALL DEPENDS ${jar_fname})
add_dependencies(jar lcmtypes_${PROJECT_NAME}_jar) #the jar depends on the lcmtypes being already built.

get_filename_component(target_name ${jar_fname} NAME_WE)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${jar_fname} DESTINATION ${JAVA_INSTALL_SUBDIR})
