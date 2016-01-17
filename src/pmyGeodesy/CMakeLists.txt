#--------------------------------------------------------
# The CMakeLists.txt for:                       pmyGeodesy
# Author(s):                              David Issa Mattos
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)


SET(SRC
  myGeodesy.cpp
  myGeodesy_Info.cpp
  main.cpp
)



ADD_EXECUTABLE(pmyGeodesy ${SRC})


set_property(TARGET pmyGeodesy PROPERTY CXX_STANDARD 11)
set_property(TARGET pmyGeodesy PROPERTY CXX_STANDARD_REQUIRED ON)

TARGET_LINK_LIBRARIES(pmyGeodesy
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

