#--------------------------------------------------------
# The CMakeLists.txt for:                       pImageTransmitt
# Author(s):                              David Mattos
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  ImageTransmitt.cpp
  ImageTransmitt_Info.cpp
  main.cpp
)



ADD_EXECUTABLE(pImageTransmitt ${SRC})

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

TARGET_LINK_LIBRARIES(pImageTransmitt
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   ${OpenCV_LIBS})
