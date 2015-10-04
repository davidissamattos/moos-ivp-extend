/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: NavIMU.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "NavIMU.h"

using namespace std;
using namespace boost::numeric::odeint;
typedef std::vector< double > state_type;

//---------------------------------------------------------
// Constructor

NavIMU::NavIMU()
{
  m_iterations = 0;
  m_timewarp   = 1;
  //inicializando as variaveis
  prefix = "";
  px=0; vx=0; ax =0; abx =0; dpx=0; vbx = 0;
  py=0; vy=0; ay =0; aby =0; dpy =0; vby = 0;
  pz=0; vz=0; az =0; abz =0;
  wx=0; wy=0; wz =0;
  phi = 0; dphi =0;
  theta = 0; dtheta =0;
  psi = pi/2; dpsi = 0;
  tempo_anterior = 0;
  tempo_atual = 0;
  for (int nCount=0; nCount < 5; nCount++)
	           x.push_back(0.0);
  x[4]=psi;
  
}

//---------------------------------------------------------
// Destructor

NavIMU::~NavIMU()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool NavIMU::OnNewMail(MOOSMSG_LIST &NewMail)
{
    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++) 
	{
	    CMOOSMsg &msg = *p;
		string key = msg.GetKey();//Nome da mensagem
	    
		if(key == "IMU_AX")
	  	{
	  		abx = msg.GetDouble();
	  	}
	    if(key == "IMU_AY")
	  	{
	  		aby = msg.GetDouble();
	  	}
	    if(key == "IMU_WZ")
	  	{
	  		wz = msg.GetDouble();
	  	}
	    if(key == "IMU_HEADING")
	  	{
	  		realheading = msg.GetDouble();
	  	}
		
	 }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool NavIMU::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool NavIMU::Iterate()
{
	m_iterations++;
    if(m_iterations!=0)
    {
		//Utiliza o modelo do barco e integra no intervalo
		deadreckoning();
		heading = (90 - psi*180/pi);//Convertendo para graus e passando para o referencial correto
		//double heading = psi*180/pi;
		while(heading<0 || heading>360)
		{
			if(heading<0){heading = heading + 360;}
			if(heading>360){heading = heading - 360;}
		}
		
		double speed = sqrt(vx*vx + vy*vy);
		//Notificando o MOOSDB		
		m_Comms.Notify(prefix+"NAV_X", px);//m
	  	m_Comms.Notify(prefix+"NAV_Y",py);//m
		m_Comms.Notify(prefix+"NAV_HEADING",heading);//degree
		m_Comms.Notify(prefix+"NAV_SPEED",speed);//m/s

		
	}
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool NavIMU::OnStartUp()
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
		  //A principio nao recebe nenhum parametro de startup
		  	if(param == "PREFIX")
			{
		   		prefix = value+"_";
			}
		}
    }
  
    m_timewarp = GetMOOSTimeWarp();

    RegisterVariables();	
    return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void NavIMU::RegisterVariables()
{
	//Registra para as variaveis do acelerometro e do giroscopio
	m_Comms.Register("IMU_AX", 0);
	m_Comms.Register("IMU_AY", 0);
	m_Comms.Register("IMU_AZ", 0);
	m_Comms.Register("IMU_WX", 0);
	m_Comms.Register("IMU_WY", 0);
	m_Comms.Register("IMU_WZ", 0);
	m_Comms.Register("IMU_HEADING", 0);
}

//---------------------------------------------------------
// Procedure: Simulate Model
// Using lambda functions 

void NavIMU::deadreckoning()
{	
	tempo_anterior = tempo_atual;
	tempo_atual = MOOSTime();
	double t = tempo_atual;
	double dt = tempo_atual-tempo_anterior;
	//Nao funciona pq o ODEINT integra o psi junto, ele tem que ser integrado antes
	//stepper.do_step([this]( const state_type &x , state_type &dxdt , const double  /*t*/)
	/*{
		
		//Todas essas variaveis estao no ECEF
			double vx = x[0];
			double vy = x[1];
			px = x[2];
			py = x[3];
			psi = x[4];
			
			dxdt[0] = abx*cos(psi) - aby*sin(psi);
			dxdt[1] = abx*sin(psi) + aby*cos(psi); 
			dxdt[2] = vx; 
			dxdt[3] = vy;
			dxdt[4] = wz;// dpsi/dt = wz
	 }, x , t , dt );
	 */
	
	//Este esta funcionando com o uTimerScript
	//Formulacao ligeiramente diferente da proposta pelo Douglas
	dvx = abx;
	dvy = aby;
	dpx = vx*cos(psi) - vy*sin(psi);
	dpy = vx*sin(psi) + vy*cos(psi);
	dpsi = wz;
	
	vx = dvx*dt + vx;
	vy = dvy*dt + vy;
	px = dpx*dt + px;
	py = dpy*dt + py;
	psi = dpsi*dt + psi;

	
	//Esse apesar de estar certo (teoricamente), mas nao esta funcionando com uTimerScript, pois nas integracoes se fizer a conta o psi que esta sendo utilizado para a integracao esta um passo atrasado, logo tem que integrar o psi antes de usar as formulas
	//Integrando utilizando Euler e o tempo entre cada iteracao
	/*
	dvx = abx*cos(psi) - aby*sin(psi);// ACEL NO NED
	dvy = abx*sin(psi) + aby*cos(psi);// ACEL NO NED
	dpx = vx;
	dpy = vy;
	dpsi = wz;
	
	vx = dvx*dt + vx;
	vy = dvy*dt + vy;
	px = dpx*dt + px;
	py = dpy*dt + py;
	psi = dpsi*dt + psi;
	*/
	
	double error = realheading - heading;
	cout<<"Erro: "<<error<<endl;
	
	
}
