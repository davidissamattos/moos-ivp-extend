/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: ITA                                             */
/*    FILE: BarcoDouglas.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <list>
#include <iterator>
#include "MBUtils.h"
#include "BarcoDouglas.h"

using namespace std;

//---------------------------------------------------------
// Constructor

BarcoDouglas::BarcoDouglas()
{
    m_iterations = 0;
	m_timewarp   = 1;
	myheading	 = 0;
}

//---------------------------------------------------------
// Destructor

BarcoDouglas::~BarcoDouglas()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool BarcoDouglas::OnNewMail(MOOSMSG_LIST &NewMail)
{
    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++) 
	{
	    CMOOSMsg &msg = *p;
		string key = msg.GetKey();//Nome da mensagem
		  //Pegar os valores de cada uma das variaveis do motor 1,0,-1
	    if(key == v_MotorM1_name)
	  	{
	  		MotorM1 = msg.GetDouble();
	  	}
	    if(key == v_MotorM2_name)
		{
			MotorM2 = msg.GetDouble();
	    }
		if(key == m_myx_name)
		{
			myx = msg.GetDouble();
		}
		if(key == m_myy_name)
		{
			myy = msg.GetDouble();
		}
		if(key == m_myspeed_name)
		{
			myspeed = msg.GetDouble();
		}
		if(key == m_myheading_name)
		{
			myheading = msg.GetDouble();
		}
		if(key == m_myturn_name)
		{
			myw = msg.GetDouble();
		}
	 }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool BarcoDouglas::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()

bool BarcoDouglas::Iterate()
{
	m_iterations++;
    if(m_iterations!=0)
    {
		//Atualiza os estados do barco com os novos vetores lidos
		updateModelVars();
		//Resolve as equacoes do barco
		myModel.updateStates();
		
		myheading = 90 - myModel.Psi;
		while(myheading<0 || myheading>360)
			{
				if(myheading<0){myheading = myheading + 360;}
				if(myheading>360){myheading = myheading - 360;}
			}
		//Notificacoes - Passa os dados de novo para o MOOSDB
	  	m_Comms.Notify("NAV_X",myModel.pos_x);
	  	m_Comms.Notify("NAV_Y",myModel.pos_y);
		m_Comms.Notify("NAV_HEADING",myheading);//Myheading possui uma diferenca de eixos coordenados em relacao ao psi do barco
		m_Comms.Notify("NAV_SPEED",myModel.speed);
		m_Comms.Notify("NAV_TURN",myModel.w);
		
		//Configurando as latitudes e longitudes
		//Local2GlobalCoordinates(myx, myy);
	  	
		//Notificacoes de debug
		//m_Comms.Notify("DELTAT_BARCO",myModel.deltat);
	 }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
// happens before connection is open

bool BarcoDouglas::OnStartUp()
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
  //Essa linha indica que a string lida do arquivo moos, se VAR_IN = variavel, o nome da variavel que usaremos chama "variavel
      		if(param == "M1")
  			{
   	       		v_MotorM1_name = value;
  			}
			if(param == "M2")
			{
   				v_MotorM2_name = value;
			}
      		if(param == "NAV_X")
  			{
   	       		m_myx_name = value;
  			}
      		if(param == "NAV_Y")
  			{
   	       		m_myy_name = value;
  			}
      		if(param == "NAV_HEADING")
  			{
   	       		m_myheading_name = value;
  			}
      		if(param == "NAV_SPEED")
  			{
   	       		m_myspeed_name = value;
  			}
      		if(param == "NAV_TURN")
  			{
   	       		m_myturn_name = value;
  			}
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

void BarcoDouglas::RegisterVariables()
{
	//Registrando para receber o update das variaveis de estado do motor e do barco
	m_Comms.Register(v_MotorM1_name, 0);
	m_Comms.Register(v_MotorM2_name, 0);
	m_Comms.Register(m_myx_name, 0);
	m_Comms.Register(m_myy_name, 0);
	m_Comms.Register(m_myheading_name, 0);
	m_Comms.Register(m_myspeed_name, 0);
	m_Comms.Register(m_myturn_name, 0);
}

void BarcoDouglas::updateModelVars()
{
	myModel.tempo_anterior = myModel.tempo;
	myModel.tempo = MOOSTime();
	myModel.deltat = myModel.tempo-myModel.tempo_anterior;
	myModel.Psi = 90-myheading;
	myModel.pos_x = myx;
	myModel.pos_y = myy;
	myModel.speed = myspeed;
	myModel.M1 = MotorM1;
	myModel.M2 = MotorM2;
	myModel.w = myw;
}