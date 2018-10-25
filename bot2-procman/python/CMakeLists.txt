find_package(PythonInterp)

if(PYTHONINTERP_FOUND AND PYTHON_VERSION_MAJOR EQUAL 2)
  option(GEN_BUILD_PREFIX "Generate build_prefix.py" ON)
  
  if(GEN_BUILD_PREFIX)
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/src/bot_procman/build_prefix.py
        "BUILD_PREFIX='${CMAKE_INSTALL_PREFIX}'\n")
      #  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/bot_procman/build_prefix.py)
  endif(GEN_BUILD_PREFIX)
  
  # install all python packages in the src/ directory
  pods_install_python_packages(${CMAKE_CURRENT_SOURCE_DIR}/src)
  
  if(TARGET ${LCM_NAMESPACE}lcm)
    get_target_property(LCM_INCLUDE_DIRS ${LCM_NAMESPACE}lcm
      INTERFACE_INCLUDE_DIRECTORIES)
    foreach(LCM_INCLUDE_DIR ${LCM_INCLUDE_DIRS})
      get_filename_component(LCM_PREFIX "${LCM_INCLUDE_DIR}" DIRECTORY)
      list(APPEND LCM_PYTHON_DIR_HINTS "${LCM_PREFIX}/${PYTHON_SITE_PACKAGES_DIR}")
    endforeach()
  endif()
  
  find_path(LCM_PYTHON_DIR NAMES "lcm/__init__.py" HINTS ${LCM_PYTHON_DIR_HINTS})
  
  if(NOT LCM_PYTHON_DIR)
    message(FATAL_ERROR "Could NOT find lcm-python")
  endif()
  
  configure_file(bot-procman-sheriff.in bot-procman-sheriff @ONLY)
  
  install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/bot-procman-sheriff"
    DESTINATION "${EXECUTABLE_INSTALL_PATH}")
  
  install(FILES procman-sheriff.glade DESTINATION share/bot_procman)
endif()
