#--------------------------------------------------------
# The CMakeLists.txt for:                       pImageXRelayDisplay
# Author(s):                              David Mattos
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  ImageXRelayDisplay.cpp
  ImageXRelayDisplay_Info.cpp
  main.cpp
)



ADD_EXECUTABLE(pImageXRelayDisplay ${SRC})
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

TARGET_LINK_LIBRARIES(pImageXRelayDisplay
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   ${OpenCV_LIBS})

