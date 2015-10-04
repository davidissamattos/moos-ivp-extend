/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: NavIMU.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef NavIMU_HEADER
#define NavIMU_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <iostream>
#include <boost/numeric/odeint.hpp>
#include <vector>
#include <cmath>
#include <ctime>
#include <eigen3/Eigen/Dense>

//Namespaces
	using namespace Eigen;
	using namespace std;
	using namespace boost::numeric::odeint;

//Definicoes typedef e constantes
	typedef std::vector< double > state_type;
	const double pi=3.1415926;

class NavIMU : public CMOOSApp
{
 public:
   NavIMU();
   ~NavIMU();

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
   
   double tempo_anterior;
   double tempo_atual;
   state_type x;
   runge_kutta4< state_type > stepper;//tipo de integracao
   
   
   //Variaveis
   double ax;
   double ay;
   double az;
   double vx; double vbx; double dvx;
   double vy; double vby; double dvy;
   double vz;
   double px; double dpx;
   double py; double dpy;
   double pz;
   double realheading;
   
   //Variaveis do giroscopio e do acelerometro
   //giroscopio no corpo
   double wx;//wx
   double wy;//wy
   double wz;//wz
   //acelerometro no corpo
   double abx;
   double aby;
   double abz;
   //Euler
   double phi;
   double theta;
   double psi;
   double dphi;
   double dtheta;
   double dpsi;
   double heading;
   
   string prefix; 
   
   void deadreckoning();

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
