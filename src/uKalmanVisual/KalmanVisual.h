/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: KalmanVisual.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef KalmanVisual_HEADER
#define KalmanVisual_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <zmq.hpp>
#include <zhelpers.hpp>
#include <thread>
#include <pthread.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;
using namespace zmq;

typedef std::vector< double > state_type;

class KalmanVisual : public CMOOSApp
{
 public:
   KalmanVisual();
   ~KalmanVisual();

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
   context_t* context;
   thread* pythonthread;
   
   //Variaveis que serao plotadas
   double pk0;
   double pk1;
   double pk2;
   double pk3;
   double pk4;
   double pk5;
   double pk6;
   double pk7;
   double xerro0;
   double xerro1;
   double xerro2;
   double xerro3;
   double xerro4;
   double xerro5;
   double xerro6;
   double xerro7;
   double t;
   double t0;
   
};

#endif 
