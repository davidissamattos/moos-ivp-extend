/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: ITA                                             */
/*    FILE: Model.cpp                                        */
/*    DATE:                                                 */
/************************************************************/





/*
Secao de duvidas e problemas
1- Modelo divergindo para w
2- Px e Py sao posicoes inerciais?
3- Vx e Vy sao velocidiades inerciais
4- Existem limites para as integrais
5- Implementacao do modelo

Ver as implementacoes finais no matlab, sera que eh possivel?

*/






#include "Model.h"
#include <iostream>

using namespace std;


////////////////////// Constructor //////////////////////////////
Model::Model()
{
	//Estados do barco
		w=0;
		dw=0;
		Vx=0;
		dVx=0;
		Vy=0;
		dVy=0;
		Px=0;
		dPx=0;
		Py=0;
		dPy=0;
		speed=0;
		pos_x=0;
		pos_y=0;
		Psi=0;
		dPsi=0;
		M1=0;
		M2=0;
		FDx=0;
		FDy=0;
		TDz=0;
	//Constantes do Barco
		m=14.81;//kg //Mass
		Fp=1.4;//N //Constante de forca do motor (forca de propulsao)
		J=3.37;//kg/m^2 //Momento de inercia
		Kx=7.025; //Constantes de arrasto
		Ky=35.12; //Constantes de arrasto
		Kz=35.12; //Constantes de arrasto
		//Dimensoes do barco
		d=0.56;//m
		//Saturacoes de velocidade angular e linear
		vmax=0.625;
		wmax=5*pi/180;//7 graus por segundo	
}

////////////////////// getTime //////////////////////////////	
void Model::getTime(double timemsg)
{
	tempo = timemsg;
}

////////////////////// integral //////////////////////////////
double Model::integral(double deltaT, double dx, double x0)
{
	double xfinal;
	xfinal = x0 + dx*deltaT;
	return xfinal;
}

double Model::integral(double deltaT, double dx, double x0, double limsup, double liminf)
{
	double xfinal;
	xfinal = x0 + dx*deltaT;
	xfinal= min(max(xfinal, liminf), limsup);
	return xfinal;
}

////////////////////// showSpeed //////////////////////////////
double Model::calcSpeed()
{
	double interm;
	interm = pow(Vx,2)+pow(Vy,2);
	interm = sqrt(interm);
	return interm;
}

////////////////////// m_dw //////////////////////////////
double Model::m_dw()
{
	double interm;
	interm=((M2*Fp - M1*Fp)*d - copysignf(1,w)*Kz*w*w)/J;
	return interm;
}

////////////////////// m_dVx //////////////////////////////
double Model::m_dVx()
{
	double interm;
	//interm = w*Vy+(copysignf(1,Vx)*(Fp*M2+Fp*M1-Kx*Vx*Vx))/m;//Esta expressao esta na tese
	interm = w*Vy+(Fp*M2+Fp*M1 - copysignf(1,Vx)*Vx*Vx*Kx)/m;
	return interm;
}

////////////////////// m_dVy //////////////////////////////
double Model::m_dVy()
{
	double interm;
	interm = -w*Vx-copysignf(1,Vy)*(Ky*Vy*Vy/m);
	return interm;
}

////////////////////// m_dPx //////////////////////////////
double Model::m_dPx()
{
	double interm;
	interm = (Vx*cos(Psi*pi/180) - Vy*sin(Psi*pi/180));
	return interm;
}

////////////////////// m_dPy //////////////////////////////
double Model::m_dPy()
{
	double interm;
	interm = (Vx*sin(Psi*pi/180) + Vy*cos(Psi*pi/180));
	return interm;
}

////////////////////// m_dPsi //////////////////////////////
double Model::m_dPsi()
{
	return w;
}

////////////////////// updateStates //////////////////////////////
void Model::updateStates()
{
	Px = pos_x;
	Py = pos_y;
	
	//calculando com os valores do modelo
	dw = m_dw();
	dVx = m_dVx();
	dVy = m_dVy();
	dPx = m_dPx();
	dPy = m_dPy();
	dPsi = m_dPsi()*180/pi;
	//resolvendo o sistema de equacoes - Valores previstos pelo modelo
	w = integral(deltat,dw,w,wmax,-wmax);
	//w = integral(deltat,dw,w);
	Vx = integral(deltat,dVx,Vx,vmax,-vmax);
	Vy = integral(deltat,dVy,Vy,vmax,-vmax);
	Px = integral(deltat,dPx,Px);
	Py = integral(deltat,dPy,Py);
	Psi = integral(deltat,dPsi,Psi);	
	
	
	//Nova posicao do barco
	pos_x = Px;
	pos_y = Py;
	
	
	speed = calcSpeed();
	
	//debug
	//cout<<"deltat "<<deltat<<endl;
	//cout<<"dpx "<<dPx<<endl;
	//cout<<"dpy "<<dPy<<endl;
	//cout<<"Vx "<<Vx<<endl;
	//cout<<"Vy "<<Vy<<endl;
	//cout<<"w "<<w<<endl;
	//cout<<"dPsi "<<dPsi<<endl;
	//cout<<"Psi "<<Psi<<endl;
	//cout<<"M1 "<<M1<<endl;
	//cout<<"M2 "<<M2<<endl;
	
}