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

#include <iostream>
#include <iterators>
#include <string>
#include <vector>
//Numerical Solver
#include <boost/numeric/odeint.hpp>



typedef std::vector< double > state_type;

//Constantes do programa
	const double pi 3.1415926;
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

	//Variavel global
	double tempo_atual=0;
	double tempo_anterior=0;



void modeloBarco(const state_type &x, state_type &dxdt, const double t, double M1, double M2)
{
	double w = x[0];
	double Vx = x[1];
	double Vy = x[2];
	double px = x[3];
	double py = x[4];
	
	dxdt[0] = ((M2*Fp - M1*Fp)*d - copysignf(1,w)*Kz*w*w)/J;//dwdt
	dxdt[1] = w*Vy+(Fp*M2+Fp*M1 - copysignf(1,Vx)*Vx*Vx*Kx)/m;//dVxdt
	dxdt[2] = -w*Vx-copysignf(1,Vy)*(Ky*Vy*Vy/m);//dVydt
	dxdt[3] = (Vx*cos(Psi*pi/180) - Vy*sin(Psi*pi/180));//dpxdt
	dxdt[4] = (Vx*sin(Psi*pi/180) + Vy*cos(Psi*pi/180));//dpydt
	dxdt[5] = w;//dpsidt
}

void integracao()
{
	runge_kutta4< state_type> stepper;
	//funcao do_step(sistema, estado x, tempoatual, passo)
	//Geralmente feito em um loop
			   // const double dt = 0.01;
			   // for( double t=0.0 ; t<10.0 ; t+= dt )
			   //   stepper.do_step( harmonic_oscillator , x ,t , dt );
	tempo_anterior = tempo_atual;
	tempo_atual = MOOSTime();
	double deltat = tempo_atual-tempo_anterior;
	
	stepper.do_step(modeloBarco,x,tempo_atual,deltat)
		//Funcao integrate e quando simula por um tempo completo
	//integrate_const(stepper, modeloBarco, x,)
}
