/************************************************************/
/*    NAME: David Issa Mattos                               */
/*    ORGN: ITA                                             */
/*    FILE: Serial.h                                        */
/*    DATE:                                                 */
/************************************************************/

#ifndef Serial_HEADER
#define Serial_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "SerialPort.h"
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision

class Serial : public CMOOSApp
{
 public:
   Serial();
   ~Serial();

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
   
   double m_m1;
   std::string m_m1_name;
   double m_m2;
   std::string m_m2_name;
   std::string serialout_string;
   std::string leituraserial;
   std::string m_serial_options;//Seleciona se o dispositivo e um arduino ou o GPS, depende se tem bit de paridade e etc
   std::string m_serial_out;
   std::string imu_string;
   std::string gps_string;
   int m_BaudRate;
   
   
	std::string temp;
	double mylat;
	double mylon;
	double mytime;
	double myheading;
	double myspeed;

 private: // Configuration variables
	SerialPort m_MySerial;
	std::string m_portDirection;

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
