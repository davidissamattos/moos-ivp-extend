#--------------------------------------------------------
# The CMakeLists.txt for:                       iSerial
# Author(s):                              David Issa Mattos
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  Serial.cpp
  Serial_Info.cpp
  main.cpp
  SerialPort.cpp #classe criada para comunicar com a porta serial do LINUX
)

ADD_EXECUTABLE(iSerial ${SRC})

FIND_PACKAGE( Boost COMPONENTS system regex filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
set_property(TARGET iSerial PROPERTY CXX_STANDARD 11)
set_property(TARGET iSerial PROPERTY CXX_STANDARD_REQUIRED ON)



TARGET_LINK_LIBRARIES(iSerial
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

