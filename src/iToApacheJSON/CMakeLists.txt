#--------------------------------------------------------
# The CMakeLists.txt for:                       iToApacheJSON
# Author(s):                              David
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  jsoncpp.cpp
  ToApacheJSON.cpp
  ToApacheJSON_Info.cpp
  main.cpp
)



ADD_EXECUTABLE(iToApacheJSON ${SRC})

TARGET_LINK_LIBRARIES(iToApacheJSON
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

