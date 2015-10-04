/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: BarcoSimu.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "BarcoSimu.h"

using namespace std;
using namespace boost::numeric::odeint;
typedef std::vector< double > state_type;
//---------------------------------------------------------
// Constructor

BarcoSimu::BarcoSimu()
{
  m_iterations = 0;
  m_timewarp   = 1;
  //inicializando o vetor com 6 posicoes
  for (int nCount=0; nCount < 6; nCount++)
	           x.push_back(0); 
  //Condicoes iniciais do modelo
  x[0] = 0.0;
  x[1] = 0.0;
  x[2] = 0.0;
  x[3] = 0.0;
  x[4] = 0.0;
  x[5] = pi/2;// Inicia com heading de zero graus
  abx=0;
  aby=0;
  abz=0;
  vbx=0;
  vby=0;
  vbz=0;
  wbx=0;
  wby=0;
  wbz=0;
  speed=0;
  heading=0;
  pos_x=0;
  pos_y=0;
  vx=0;
  vy=0;
  wspeed=0;
  ax=0;
  ay=0;
  tempo_anterior = 0;
  tempo_atual = 0;
}

//---------------------------------------------------------
// Destructor

BarcoSimu::~BarcoSimu()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool BarcoSimu::OnNewMail(MOOSMSG_LIST &NewMail)
{
    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++) 
	{
	    CMOOSMsg &msg = *p;
		string key = msg.GetKey();//Nome da mensagem
		  //Pegar os valores de cada uma das variaveis do motor 1,0,-1
	    if(key == "M1")
	  	{
	  		M1 = msg.GetDouble();
	  	}
	    if(key == "M2")
	  	{
	  		M2 = msg.GetDouble();
	  	}
	 }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool BarcoSimu::OnConnectToServer()
{	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool BarcoSimu::Iterate()
{
	m_iterations++;
    if(m_iterations!=0)
    {
		//Utiliza o modelo do barco e integra no intervalo
		integrar_passo();
		
		//Atualizando as variaveis que serao postadas no MOOSDB
			wbz = x[0];
			pos_x = x[3];//em m
			pos_y = x[4];//em m
			speed = sqrt(vx*vx+vy*vy);//em m/s
			
			heading = (90 - x[5]*180/pi);//Convertendo para graus e passando para o referencial correto
			while(heading<0 || heading>360)
			{
				if(heading<0){heading = heading + 360;}
				if(heading>360){heading = heading - 360;}
			}
		//Notificando o MOOSDB
		//ECEF	
	  	m_Comms.Notify("NAV_X", pos_x);//m
	  	m_Comms.Notify("NAV_Y",pos_y);//m
		m_Comms.Notify("NAV_HEADING",heading);//degree
		m_Comms.Notify("NAV_SPEED",speed);//m/s
		//SISTEMA CORPO
		m_Comms.Notify("NAV_IMUWZ",wbz);//rad/s
		m_Comms.Notify("NAV_IMUAX",abx);
		m_Comms.Notify("NAV_IMUAY",aby);
		m_Comms.Notify("NAV_VX",vx);
		m_Comms.Notify("NAV_VY",vy);
		
	}
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool BarcoSimu::OnStartUp()
{
  cout<<"Startup"<<endl;
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
  	if(param == "POSX")
	{
	  std::string valor = value;
	  x[3] = std::stod (valor.c_str());
	}
  	if(param == "POSY")
	{
	  std::string valor = value;
	  x[4] = std::stod (valor.c_str());
	}
  	if(param == "HEADING")
	{
	  std::string valor = value;
	  x[5] = (90-std::stod (valor.c_str()))*pi/180;
	}
    }
  }
  
  m_timewarp = GetMOOSTimeWarp();

  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void BarcoSimu::RegisterVariables()
{
	m_Comms.Register("M1", 0);
	m_Comms.Register("M2", 0);
}

//---------------------------------------------------------
// Procedure: Simulate Model
// Using lambda functions 

void BarcoSimu::integrar_passo()
{	
	tempo_anterior = tempo_atual;
	tempo_atual = MOOSTime();
	double t = tempo_atual;
	double dt = tempo_atual-tempo_anterior;
	stepper.do_step([this]( const state_type &x , state_type &dxdt , const double  /*t*/)
	{
			double w = x[0];
			double Vx = x[1];
			double Vy = x[2];
			double px = x[3];
			double py = x[4];
			double psi = x[5];
			
			dxdt[0] = ((M2*Fp - M1*Fp)*d - copysignf(1,w)*Kz*w*w)/J;//dwdt
			dxdt[1] = w*Vy+(Fp*M2+Fp*M1 - copysignf(1,Vx)*Vx*Vx*Kx)/m;//dVbxdt -> CORPO
			dxdt[2] = -w*Vx-copysignf(1,Vy)*(Ky*Vy*Vy/m);//dVbydt - CORPO
			dxdt[3] = (Vx*cos(psi) - Vy*sin(psi));//dpexdt -> ECEF
			dxdt[4] = (Vx*sin(psi) + Vy*cos(psi));//dpeydt -> ECEF
			dxdt[5] = w;//dpsidt
			
			abx = dxdt[1];
			aby = dxdt[2];
			vx = dxdt[3];
			vy = dxdt[4];
	 }, x , t , dt );
}
