/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: BarcoSimu3Kalman.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "BarcoSimu3Kalman.h"

using namespace std;
using namespace boost::numeric::odeint;
typedef std::vector< double > state_type;

std::default_random_engine eng;


//---------------------------------------------------------
// Constructor

BarcoSimu3Kalman::BarcoSimu3Kalman()
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
  wz=0;
  speed=0;
  heading=0;
  gpsx=0;
  gpsy=0;
  vx=0;
  vy=0;
  ax=0;
  ay=0;
  tempo_anterior = 0;
  tempo_atual = 0;
  
  ic =0;
}

//---------------------------------------------------------
// Destructor

BarcoSimu3Kalman::~BarcoSimu3Kalman()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool BarcoSimu3Kalman::OnNewMail(MOOSMSG_LIST &NewMail)
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

bool BarcoSimu3Kalman::OnConnectToServer()
{	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool BarcoSimu3Kalman::Iterate()
{
	m_iterations++;
    if(m_iterations!=0)
    {
		//Utiliza o modelo do barco e integra no intervalo
		integrar_passo();
		
		
		//std::tr1::mt19937_64 eng;  // a core engine class Mersene Twist 64b
		std::normal_distribution<double> gpsx_dist(0,sigma_gpsx);
		std::normal_distribution<double> gpsy_dist(0,sigma_gpsy);
		std::normal_distribution<double> heading_dist(0,sigma_heading);
		std::normal_distribution<double> ax_dist(bias_ax,sigma_ax);
		std::normal_distribution<double> ay_dist(bias_ay,sigma_ay);
		std::normal_distribution<double> wz_dist(bias_wz,sigma_wz);
		
		speed = sqrt(vx*vx+vy*vy);//em m/s
		//Acrescentando os erros de GPS e de IMU
		wz = x[0] + wz_dist(eng);
		gpsx = x[3] + gpsx_dist(eng);//em m
		cout << gpsx<<"    "<<gpsy<<endl;
		gpsy = x[4]+ gpsy_dist(eng);//em m
		abx = abx + ax_dist(eng);
		aby = aby + ay_dist(eng);
		double heading_v = (90 - x[5]*180/pi);
		heading = (90 - x[5]*180/pi) + heading_dist(eng);//Convertendo para graus e passando para o referencial correto
		while(heading<0 || heading>360)
		{
			if(heading<0){heading = heading + 360;}
			if(heading>360){heading = heading - 360;}
		}
		cout << gpsx<<"    "<<gpsy<<"	"<<heading<<endl;
		
		
		m_Comms.Notify("NAV_HEADINGV", heading_v);//m
		m_Comms.Notify("NAV_XV", x[3]);//m
		m_Comms.Notify("NAV_YV", x[4]);//m
		//Para usar com o filtro de Kalman
		m_Comms.Notify("GPS_X", gpsx);//m
	  	m_Comms.Notify("GPS_Y",gpsy);//m
		m_Comms.Notify("IMU_HEADING",heading);//degree
		m_Comms.Notify("IMU_WZ",wz);//rad/s
		m_Comms.Notify("IMU_AX",abx);
		m_Comms.Notify("IMU_AY",aby);
		
	  	
		
	}
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool BarcoSimu3Kalman::OnStartUp()
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
	  	if(param == "SIGMA_GPSX")
		{
		  std::string valor = value;
		  sigma_gpsx = std::stod (valor.c_str());
		}
		if(param == "SIGMA_GPSY")
		{
  		  std::string valor = value;
  		  sigma_gpsy = std::stod (valor.c_str());
		}
		if(param == "SIGMA_AX")
		{
  		  std::string valor = value;
  		  sigma_ax = std::stod (valor.c_str());
		}
		if(param == "SIGMA_AY")
		{
  		  std::string valor = value;
  		  sigma_ay = std::stod (valor.c_str());
		}
		if(param == "SIGMA_WZ")
		{
  		  std::string valor = value;
  		  sigma_wz = std::stod (valor.c_str());
		}
		if(param == "SIGMA_HEADING")
		{
  		  std::string valor = value;
  		  sigma_heading = std::stod (valor.c_str());
		}
		if(param == "BIAS_AX")
		{
  		  std::string valor = value;
  		  bias_ax = std::stod (valor.c_str());
		}
		if(param == "BIAS_AY")
		{
  		  std::string valor = value;
  		  bias_ay = std::stod (valor.c_str());
		}
		if(param == "BIAS_WZ")
		{
  		  std::string valor = value;
  		  bias_wz = std::stod (valor.c_str());
		}
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

void BarcoSimu3Kalman::RegisterVariables()
{
	m_Comms.Register("M1", 0);
	m_Comms.Register("M2", 0);
}

//---------------------------------------------------------
// Procedure: Simulate Model
// Using lambda functions 

void BarcoSimu3Kalman::integrar_passo()
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
