#--------------------------------------------------------
# The CMakeLists.txt for:                       uBarcoDouglas
# Author(s):                              David Issa Mattos
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(uBarcoDouglas ${SRC})

TARGET_LINK_LIBRARIES(uBarcoDouglas
   ${MOOS_LIBRARIES}
   contacts
   geometry 
   apputil
   mbutil
   m
   pthread)

