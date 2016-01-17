#I cloned and compiled the code, and tried to run with python 2.7.10, but get the following error immediately upon creation of the pymoos.comms() object.

#Fatal Python error: PyEval_SaveThread: NULL tstate
#Abort trap: 6

#Am I missing something that needed to be done when compiling?
#@mandad
#mandad commented on Jul 29
#I fixed the problem, it had to do with compiling against a different version of Python than I was running the application (homebrew python on mac was not used as the default). It works with cmake -DPYTHON_LIBRARY="/usr/local/Cellar/python/2.7.10_2/Frameworks/Python.framework/Versions/2.7/liblibpython2.7.dylib" ../run in the build directory (or whatever you python version is).

cmake_minimum_required(VERSION 2.6)
project (python-moos)
find_package(MOOS 10 REQUIRED)

find_package(PythonLibs)
if(PYTHONLIBS_FOUND)
  include_directories("${PYTHON_INCLUDE_DIRS}")
else()
  message(FATAL_ERROR "Unable to find PythonLibs.")
endif()

find_package(Boost REQUIRED python)
set(Boost_USE_STATIC_LIBS     ON)
set(Boost_USE_MULTITHREADED    ON)
set(Boost_USE_STATIC_RUNTIME     ON)

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
endif()
if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
endif()


include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${MOOS_INCLUDE_DIRS})

set(src
pyMOOS.cpp
) 

add_library(pymoos SHARED ${src})

set_target_properties( pymoos
    PROPERTIES
    SUFFIX ".so"
)

set_target_properties(pymoos
  PROPERTIES
  PREFIX "")
  
  
file(GLOB ExampleFiles ${CMAKE_SOURCE_DIR}/Documentation/examples/*.py)
add_custom_target(copy)
get_target_property(pymoosLocation pymoos LOCATION)
get_filename_component(pymoosDir ${pymoosLocation} PATH)
foreach(ExampleFile ${ExampleFiles})
  add_custom_command(TARGET copy PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${ExampleFile} ${pymoosDir})
endforeach()
add_dependencies(pymoos copy)    


TARGET_LINK_LIBRARIES(pymoos ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${MOOS_LIBRARIES})
