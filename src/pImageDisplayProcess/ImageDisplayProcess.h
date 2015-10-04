/************************************************************/
/*    NAME: David Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: ImageDisplayProcess.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef ImageDisplayProcess_HEADER
#define ImageDisplayProcess_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;


class ImageDisplayProcess : public CMOOSApp
{
 public:
   ImageDisplayProcess();
   ~ImageDisplayProcess();

 protected:
     bool OnNewMail(MOOSMSG_LIST &NewMail);
     bool Iterate();
     bool OnConnectToServer();
     bool OnStartUp();
     void RegisterVariables();
     long double image_size;
     Mat imagem;

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
