/************************************************************/
/*    NAME: David Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: ImageTransmitt.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef ImageTransmitt_HEADER
#define ImageTransmitt_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

class ImageTransmitt : public CMOOSApp
{
 public:
   ImageTransmitt();
   ~ImageTransmitt();

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
   bool saveJPG(string name, Mat frame, int quality);
   Mat CaptureWebcam();

 private: // Configuration variables

 private: // State variables
	unsigned int m_iterations;
	double       m_timewarp;
	unsigned long int m_tally_recd;
	unsigned long int m_tally_sent;
	std::string       m_incoming_var;
	std::string       m_outgoing_var;
	int camera_number;
	Mat interm;
	VideoCapture cap;
};

#endif 
