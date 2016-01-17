#--------------------------------------------------------
# The CMakeLists.txt for:                       pKalmanSensorFusion
# Author(s):                              David Issa Mattos
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(pKalmanSensorFusion ${SRC})

#Configurando para incluir a biblioteca boost e eigen3
FIND_PACKAGE( Boost COMPONENTS system regex filesystem REQUIRED )
link_directories(/usr/local/lib)
INCLUDE_DIRECTORIES(usr/local/include/eigen3)
INCLUDE_DIRECTORIES(usr/local/include)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
set_property(TARGET pKalmanSensorFusion PROPERTY CXX_STANDARD 11)
set_property(TARGET pKalmanSensorFusion PROPERTY CXX_STANDARD_REQUIRED ON)


TARGET_LINK_LIBRARIES(pKalmanSensorFusion
    ${MOOS_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    contacts
    geometry 
    apputil
    mbutil
    m
    pthread)

