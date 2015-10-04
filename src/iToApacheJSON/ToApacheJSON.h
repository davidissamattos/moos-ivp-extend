/************************************************************/
/*    NAME: David                                              */
/*    ORGN: MIT                                             */
/*    FILE: ToApacheJSON.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef ToApacheJSON_HEADER
#define ToApacheJSON_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class ToApacheJSON : public CMOOSApp
{
 public:
   ToApacheJSON();
   ~ToApacheJSON();

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
