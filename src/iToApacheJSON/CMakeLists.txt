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
FIND_PACKAGE( Boost COMPONENTS system regex filesystem REQUIRED )
link_directories(/usr/local/lib)
INCLUDE_DIRECTORIES(usr/local/include/eigen3)
INCLUDE_DIRECTORIES(usr/local/include)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
set_property(TARGET iToApacheJSON PROPERTY CXX_STANDARD 11)
set_property(TARGET iToApacheJSON PROPERTY CXX_STANDARD_REQUIRED ON)

TARGET_LINK_LIBRARIES(iToApacheJSON
   ${MOOS_LIBRARIES}
   ${Boost_LIBRARIES}
   ${Boost_FILESYSTEM_LIBRARY}
   ${Boost_SYSTEM_LIBRARY}
   ${Boost_REGEX_LIBRARY}
   mbutil
   m
   pthread)

