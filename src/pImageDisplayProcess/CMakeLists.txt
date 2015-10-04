#--------------------------------------------------------
# The CMakeLists.txt for:                       pImageDisplayProcess
# Author(s):                              David Mattos
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  ImageDisplayProcess.cpp
  ImageDisplayProcess_Info.cpp
  main.cpp
)



ADD_EXECUTABLE(pImageDisplayProcess ${SRC})

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

TARGET_LINK_LIBRARIES(pImageDisplayProcess
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   ${OpenCV_LIBS})