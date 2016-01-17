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

TARGET_LINK_LIBRARIES(iSerial
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

