#--------------------------------------------------------
# The CMakeLists.txt for:                       uBarcoSimu3Kalman
# Author(s):                              David Issa Mattos
#--------------------------------------------------------

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(uBarcoSimu3Kalman ${SRC})


#Configurando para incluir a biblioteca boost
FIND_PACKAGE( Boost COMPONENTS system regex filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
set_property(TARGET uBarcoSimu3Kalman PROPERTY CXX_STANDARD 11)
set_property(TARGET uBarcoSimu3Kalman PROPERTY CXX_STANDARD_REQUIRED ON)

TARGET_LINK_LIBRARIES(uBarcoSimu3Kalman
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

