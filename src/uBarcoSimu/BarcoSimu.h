/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: BarcoSimu.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef BarcoSimu_HEADER
#define BarcoSimu_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <iostream>
#include <boost/numeric/odeint.hpp>
#include <vector>
#include <cmath>
#include <ctime>


using namespace std;
using namespace boost::numeric::odeint;
typedef std::vector< double > state_type;

//Constantes do modelo
	const double pi=3.1415926;
	const double m=14.81;//kg //Mass
	const double Fp=1.4;//N //Constante de forca do motor (forca de propulsao)
	const double J=3.37;//kg/m^2 //Momento de inercia
	const double Kx=7.025; //Constantes de arrasto
	const double Ky=35.12; //Constantes de arrasto
	const double Kz=35.12; //Constantes de arrasto
	//Dimensoes do barco
	const double d=0.56;//m
	//Saturacoes de velocidade angular e linear
	const double vmax=0.625;
	const double wmax=5*pi/180;//7 graus por segundo	
	//Constante de intervalo de tempo para integracao
	//const double dt = 0.1;



class BarcoSimu : public CMOOSApp
{
 public:
   BarcoSimu();
   ~BarcoSimu();

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
   
   
   
   //Metodos e variaveis proprios do modelo
   //state_type x(6) {0,0,0,0,0,0};
   state_type x;
   runge_kutta4< state_type > stepper;//tipo de integracao
   double M1;
   double M2;
   double tempo_anterior;
   double tempo_atual;
   
   //Essas variaveis serao as postadas no MOOSDB 
   //ECEF
   double speed;
   double heading;
   double pos_x;
   double pos_y;
   double vx;
   double vy;
   double wspeed;
   double ax;
   double ay;
   
   //REFERENCIAL DO CORPO
   double abx;
   double aby;
   double abz;
   double vbx;
   double vby;
   double vbz;
   double wbx;
   double wby;
   double wbz;
   
   void integrar_passo();

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
