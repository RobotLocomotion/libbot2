find_package(PythonInterp)

if(PYTHONINTERP_FOUND AND PYTHON_VERSION_MAJOR EQUAL 2)
  # install all python packages in the src/ directory
  pods_install_python_packages(${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()
