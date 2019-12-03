find_package(PythonInterp 3.6 MODULE)

if(PYTHONINTERP_FOUND)
  pods_install_python_packages(${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()
