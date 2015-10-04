#--------------------------------------------------------
# The CMakeLists.txt for:                       uKalmanVisual
# Author(s):                              David Issa Mattos
#--------------------------------------------------------

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
# FILE(GLOB SRC *.cpp)

SET(SRC
  KalmanVisual.cpp
  KalmanVisual_Info.cpp
  main.cpp
)

#Configurando para incluir a biblioteca boost
FIND_LIBRARY(ZMQ_LIB libzmq)
FIND_PACKAGE( Boost COMPONENTS system regex filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
set_property(TARGET uBarcoSimu PROPERTY CXX_STANDARD 11)
set_property(TARGET uBarcoSimu PROPERTY CXX_STANDARD_REQUIRED ON)

ADD_EXECUTABLE(uKalmanVisual ${SRC})

TARGET_LINK_LIBRARIES(uKalmanVisual
	zmq
   ${MOOS_LIBRARIES}
   ${Boost_LIBRARIES}
   ${Boost_FILESYSTEM_LIBRARY}
   ${Boost_SYSTEM_LIBRARY}
   ${Boost_REGEX_LIBRARY}
   contacts
   geometry 
   apputil
   mbutil
   m
   pthread)

