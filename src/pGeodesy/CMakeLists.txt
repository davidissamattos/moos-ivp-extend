#--------------------------------------------------------
# The CMakeLists.txt for:                       pGeodesy
# Author(s):                              David Issa Mattos
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  Geodesy.cpp
  Geodesy_Info.cpp
  main.cpp
)



ADD_EXECUTABLE(pGeodesy ${SRC})

set_property(TARGET pGeodesy PROPERTY CXX_STANDARD 11)
set_property(TARGET pGeodesy PROPERTY CXX_STANDARD_REQUIRED ON)

TARGET_LINK_LIBRARIES(pGeodesy
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

