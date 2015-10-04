/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: ITA                                             */
/*    FILE: Model.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef _Model
#define _Model

#include <cstring>
#include <cmath>
#include <ctime>
#define pi 3.14159

class Model
{
	
//////////////////////////// Public /////////////////////////
public:	
//Definindo o Constructor
	Model();
//Definindo algumas funcoes basicas para calculo matematico
	void getTime(double timemsg);
	void initiate();
	double showTime(){return tempo;};
	double showSpeed(){return speed;};
	
	double pos_x;//posicao em x
	double pos_y;//posicao em y
    double speed;//velocidade total inercial
	double Psi;//angulo
	double w;//velocidade angular do barco
	time_t tempo;
	time_t tempo_anterior;
	double deltat;
	double M1;// Motor esquerdo Assume 3 valores -1,0,1
	double M2;//Motor direito Assume 3 valores -1,0,1
	void updateStates();//Update das novas posicoes calculadas para proxima iteracao
	
		
		
//////////////////////////// Private /////////////////////////
private:
		
	//variaveis atuais do barco
	double Px;//pos inercial em x
	double Py;//pos inercial em y
	double Vx;//velocidade inercial em x
	double Vy;//velocidade inercial em y
	double FDx;//Forca de arrasto em x
	double FDy;//Forca de arrasto em y
	double TDz;//Torque de arrasto em z 
	
	//variaveis do vetor de estado xponto
	double dw;
	double dVx;
	double dVy;
	double dPx;
	double dPy;
	double dPsi;
	
	//Atualizacao dos estados (sao as equacoes)
	double m_dw();
	double m_dVx();//velocidade inercial
	double m_dVy();//velocidade inercial
	double m_dPx();//posicao inercial
	double m_dPy();//posicao inercial
	double m_dPsi();
	double calcSpeed();
	
	//Integrais
	double integral(double deltaT, double dx, double x);
	double integral(double deltaT, double dx, double x, double limsup, double liminf);

		
//Definindo as variaveis que serao utilizadas pelo modelo
//Variaveis do modelo
	//Massa do barco
		double m;//kg
	//Constante de forca do motor (forca de propulsao)
		double Fp;//N
	//Momento de inercia
		double J;//kg/m^2
	//Constantes de arrasto
		double Kx;
		double Ky;
		double Kz;
	//Dimensoes do barco
		double d;//m
	//Restricoes do barco
		double vmax;
		double wmax;
};

#endif 
