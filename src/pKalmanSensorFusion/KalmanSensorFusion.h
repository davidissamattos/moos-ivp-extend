/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: KalmanSensorFusion.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef KalmanSensorFusion_HEADER
#define KalmanSensorFusion_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

#include <stdio.h>
#include <iostream>
#include <boost/numeric/odeint.hpp>
#include <vector>
#include <stdlib.h>     /* atof */
#include <math.h>       /* sin */
#include <ctime>
#include <eigen3/Eigen/Dense>
#include <sstream> //Passing string through startup


//Namespaces
	using namespace Eigen;
	using namespace std;
	using namespace boost::numeric::odeint;
	
//Definicoes typedef e constantes
	typedef std::vector< double > state_type;
	const double pi=3.1415926;

class KalmanSensorFusion : public CMOOSApp
{
 public:
   KalmanSensorFusion();
   ~KalmanSensorFusion();

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
   void KalmanFilter();

 private:
	 //Vetores
	VectorXd XK_INS;
	VectorXd XK1_INS;
	VectorXd XK_INS_mais;// [Vx_ecef Vy_ecef Px_ecef Py_ecef Psi_ecef]
	VectorXd XK_INS_menos;// [Vx_ecef Vy_ecef Px_ecef Py_ecef Psi_ecef]
	VectorXd XK1_INS_menos;// [Vx_ecef Vy_ecef Px_ecef Py_ecef Psi_ecef]
	VectorXd X_ERRO;// Vetor de erros do modelo
	VectorXd Y_ERRO;// Vetor de erros do modelo
	VectorXd BiasK_menos;// [Bias_ax Bias_ay Bias_wz]
	VectorXd BiasK_mais;// [Bias_ax Bias_ay Bias_wz]
	VectorXd BiasK;// [Bias_ax Bias_ay Bias_wz]
	VectorXd BiasK1;// [Bias_ax Bias_ay Bias_wz]
	VectorXd P0_diag;//Diagonal da matriz de covariancia do erro [alpha1 alpha2 ... alpha8]
	VectorXd U_IMU; //Leitura da IMU [axbm_body aybm_body wzm]
	VectorXd U_INS; // U_IMU - Bias_INS
	VectorXd GPS; // Leitura do GPS
	VectorXd VarIMU;//Variancia da IMU [Axb Ayb Wz]
	VectorXd VarGPS;//Variancia do GPS [GPSX GPSY]
	VectorXd VarCOMP;//Variancia da Bussola
	
	
	
	MatrixXd C_ERROS;//Matriz C do espaco de estados
	MatrixXd C_ERROS_T;//Matriz do espaco de estados Transposta
	
	//Matrizes do filtro de kalman
	
	
	MatrixXd PK_menos;//Matriz diagonal do P
	MatrixXd PK_mais;
	MatrixXd PK1_menos; //P(k+1)
	MatrixXd PK; //P(k+1)
	MatrixXd PK1;
	MatrixXd P0;
	MatrixXd QK;
	
	MatrixXd RK; //matriz 3x3 dos ruidos usados no FK
	
	MatrixXd I8;//Identidade 8x8
	
	//Variaveis
	double compass;
	double psi_compass;
	double dt_accel;
	double dt_gyro;
	double T;
	
	double ic;//Contagem de iteracoes para o GPS
	double Ngps;
	double Tgps;
	
	// Initialization parameter
	double KalmanBeta;
	double GPSvar;
	double COMPASSvar;
	double ACCELvar;
	double GYROvar;
	   
    state_type x;
    runge_kutta4< state_type > stepper;//tipo de integracao
    double tempo_anterior;
    double tempo_atual;

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
