#--------------------------------------------------------
# The CMakeLists.txt for:                 pImageXRelay
# Author(s):                              David Mattos
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  ImageXRelay.cpp
  ImageXRelay_Info.cpp
  main.cpp
)



ADD_EXECUTABLE(pImageXRelay ${SRC})

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


TARGET_LINK_LIBRARIES(pImageXRelay
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   ${OpenCV_LIBS})
