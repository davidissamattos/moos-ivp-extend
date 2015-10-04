/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: ITA                                            */
/*    FILE: ControleBarco.cpp                                        */
/*    DATE:                                                 */
/************************************************************/



/*
Secao de duvidas e problemas
1- Implementacao da mudanca da proa com a tabela de acionamentos
2- Como conseguir delta_d <0

*/


#include <list>
#include <iterator>
#include "MBUtils.h"
#include "ControleBarco.h"

using namespace std;

//---------------------------------------------------------
// Constructor

ControleBarco::ControleBarco()
{
    m_iterations = 0;
    m_timewarp   = 1;
	desired_heading=0;
	myheading=0;
	desired_d=0;
	myd=0;
	myx=0;
	myy=0;
	desired_x=0;
	desired_y=0;
	delta_d=0;
	delta_heading=0;
	M1=0;//variaveis da tabela de acionamento
	M2=0;
	flag_proa=0;
	disable_controlebarco="false";
	
	//Distancia para o controlador do barco
	d=2;
}

//---------------------------------------------------------
// Destructor

ControleBarco::~ControleBarco()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool ControleBarco::OnNewMail(MOOSMSG_LIST &NewMail)
{
	
	MOOSMSG_LIST::iterator p; 
	for(p=NewMail.begin(); p!=NewMail.end(); p++) 
	{
		
		CMOOSMsg &msg = *p;
		string key = msg.GetKey();//Nome da mensagem
		//Pega os valores das variaveis
		if(key == "NAV_HEADING")
		{
			myheading = msg.GetDouble();
			cout<<"myHeadingRecebida: "<<myheading<<endl;
		}
		if(key == "DESIRED_SPEED")
		{
			//temp = msg.GetString();
			//desired_speed = std::stod(temp.c_str(),0);
			desired_speed = msg.GetDouble();
		}
		if(key == "DESIRED_HEADING")
		{
			//temp = msg.GetString();
			//desired_heading = std::stod(temp.c_str(),0);
			desired_heading = msg.GetDouble();
		}
		if(key == "DISABLE_CONTROLEBARCO")
		{
			disable_controlebarco = msg.GetString();
			toLower(disable_controlebarco);
		}		
	}	
	return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool ControleBarco::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()

bool ControleBarco::Iterate()
{
   // happens AppTick times per second
	m_iterations++;
    if(m_iterations!=0)
    {
		if(disable_controlebarco != "true")//Quando o controle esta desabilitado
		{
			tempo_ant = tempo_atual;
			tempo_atual = MOOSTime();
			deltat=tempo_atual-tempo_ant;
			Tabela();//Calulo dos valores de M1 e M2
	    	m_Comms.Notify("M1",M1);
	    	m_Comms.Notify("M2",M2);
		}
    }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
// happens before connection is open

bool ControleBarco::OnStartUp()
{
    list<string> sParams;
    m_MissionReader.EnableVerbatimQuoting(false);
    if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) 
    {
      	list<string>::iterator p;
      	for(p=sParams.begin(); p!=sParams.end(); p++) 
      	{
        	string original_line = *p;
     		string param = stripBlankEnds(toupper(biteString(*p, '=')));
        	string value = stripBlankEnds(*p);
  	   }
   }
  //inicializacao de variaveis
    m_iterations=0;
    m_timewarp = GetMOOSTimeWarp();
	RegisterVariables();	
	return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void ControleBarco::RegisterVariables()
{

	m_Comms.Register("NAV_HEADING", 0);
	m_Comms.Register("DESIRED_HEADING", 0);
	m_Comms.Register("DESIRED_SPEED", 0);
	m_Comms.Register("DISABLE_CONTROLEBARCO",0);
}

//---------------------------------------------------------
// Calcula a distancia d ate o target
double ControleBarco::calcd(double x, double y)
{
	double interm;
	interm = pow(x,2)+pow(y,2);
	interm = sqrt(interm);
	return interm;
}

//---------------------------------------------------------
// Implementacao da tabela de acionamento
void ControleBarco::Tabela()
{
	//calculo dos erros
	desired_vx = sin(desired_heading*pi/180)*desired_speed;
	desired_vy = cos(desired_heading*pi/180)*desired_speed;
	desired_x = desired_vx*deltat;
	desired_y = desired_vy*deltat;
	delta_d = calcd(desired_x,desired_y);
	delta_heading = desired_heading-(myheading);
	//cout<<"My Heading: "<<myheading<<endl;
	//delta_heading = (atan2((desired_x-myx),(desired_y-myy))-(myheading*(pi/180)))*(180/pi);
	
	//Calculo do erro de guinada -> Deve ficar entre -180 e +180 -> Convertendo
	while(delta_heading<-180 || delta_heading>180)
	{
		if(delta_heading<-180){delta_heading = delta_heading + 360;}
		if(delta_heading>180){delta_heading = delta_heading - 360;}
	}
	//cout<<"desired_vx: "<<desired_vx<<endl;
	//cout<<"desired_vy: "<<desired_vy<<endl;
	//cout<<"delta_d: "<< delta_d<<endl;
	//cout<<"desired_heading: "<<desired_heading<<endl;
	//cout<<"myheading: "<<myheading<<endl;
	//cout<<"delta_heading: "<<delta_heading<<endl;
	
	//Tabela reduzida sem levar em consideracao a mudanca de proa
	//Barco proximo do ponto
	
	if((delta_d >= 0) && (delta_d <= d) && (delta_heading>5 && delta_heading<180))/**/
	{
		M1=1;
		M2=-1;
	}
	if((delta_d >=0 ) && (delta_d <= d) && (delta_heading>0 && delta_heading<5))
	{
		M1=0;
		M2=0;
	}
	if((delta_d >=0 ) && (delta_d <= d) && (delta_heading<0 && delta_heading>-5))
	{
		M1=0;
		M2=0;
	}
	if((delta_d >= 0) && (delta_d <= d) && (delta_heading<-5 && delta_heading>-180))/**/
	{
		M1=-1;
		M2=1;
	}
	
	//Barco Longe do ponto
	//caso 1,2
	if((delta_d>d) && (delta_heading>5 && delta_heading<180))
	{
		M1=1;//Alterando a tabela proposta
		M2=0;
	}
	if((delta_d>d) && (delta_heading>0 && delta_heading<5))
	{
		M1=1;
		M2=1;
	}
	if((delta_d>d) && (delta_heading<0 && delta_heading>-5))
	{
		M1=1;
		M2=1;
	}
	if((delta_d>d) && (delta_heading<-5 && delta_heading>-180))
	{
		M1=0;//Alterando a tabela proposta
		M2=1;
	}
	
	//Condicao para que ele nao fique tentando ajustar o angulo
	if(desired_speed <= 0.01)
	{
		M1=0;
		M2=0;
	}
	
}



//To Upper e To Lower functions

void toUpper(basic_string<char>& s) {
   for (std::basic_string<char>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = toupper(*p); // toupper is for char
   }
}

void toUpper(basic_string<wchar_t>& s) {
   for (std::basic_string<wchar_t>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = towupper(*p); // towupper is for wchar_t
   }
}

void toLower(basic_string<char>& s) {
   for (std::basic_string<char>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = tolower(*p);
   }
}

void toLower(basic_string<wchar_t>& s) {
   for (std::basic_string<wchar_t>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = towlower(*p);
   }
}