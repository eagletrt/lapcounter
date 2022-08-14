cmake_minimum_required(VERSION 3.10)

project(lapcounter)

set(LAPCOUNTER_SOURCE
  lapcounter.c
  math/point.c
)

add_library(lapcounter ${LAPCOUNTER_SOURCE})

# SWIG

# find_package(PythonLibs)
# include_directories(${PYTHON_INCLUDE_PATH})
# include_directories(${PROJECT_SOURCE_DIR})
# 
# find_package(SWIG REQUIRED)
# include(${SWIG_USE_FILE})
# 
# set_source_files_properties(lapcounter.i PROPERTIES CPLUSPLUS ON)
# 
# swig_add_library(lapcounter_py LANGUAGE python SOURCES lapcounter.i)
# swig_link_libraries(lapcounter_py ${PYTHON_LIBRARIES} lapcounter)