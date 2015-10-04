##############################################################################
# FILE:        moos-ivp-extend/src/CMakeLists.txt
# DATE:        2010/09/07
# DESCRIPTION: CMakeLists.txt file for the moos-ivp-extend source directory
##############################################################################


#============================================================================
# Add the libraries in the current directory to the include path
#============================================================================
FILE(GLOB LOCAL_LIBRARY_DIRS ./lib_*)
INCLUDE_DIRECTORIES(${LOCAL_LIBRARY_DIRS})

#============================================================================
# List the subdirectories to build...
#============================================================================
ADD_SUBDIRECTORY(lib_behaviors-test)
ADD_SUBDIRECTORY(iSerial)
ADD_SUBDIRECTORY(uBarcoDouglas)
ADD_SUBDIRECTORY(pControleBarco)
#ADD_SUBDIRECTORY(pImageXRelay)
#ADD_SUBDIRECTORY(pImageXRelayDisplay)
ADD_SUBDIRECTORY(pmyGeodesy)
ADD_SUBDIRECTORY(pGeodesy)
#ADD_SUBDIRECTORY(pImageTransmitt)
#ADD_SUBDIRECTORY(pImageDisplayProcess)
ADD_SUBDIRECTORY(pManual)
ADD_SUBDIRECTORY(uBarcoSimu)
ADD_SUBDIRECTORY(uBarcoSimu2Kalman)
ADD_SUBDIRECTORY(uBarcoSimu3Kalman)
ADD_SUBDIRECTORY(uKalmanVisual)
ADD_SUBDIRECTORY(pKalmanSensorFusion)
ADD_SUBDIRECTORY(pNavIMU)
ADD_SUBDIRECTORY(iToApacheJSON)




##############################################################################
#                           END of CMakeLists.txt
##############################################################################

