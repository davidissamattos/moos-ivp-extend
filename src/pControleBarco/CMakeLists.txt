#--------------------------------------------------------
# The CMakeLists.txt for:                       pControleBarco
# Author(s):                              David Issa Mattos
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(pControleBarco ${SRC})

TARGET_LINK_LIBRARIES(pControleBarco
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

