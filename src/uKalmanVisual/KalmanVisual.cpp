/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: KalmanVisual.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "KalmanVisual.h"

using namespace std;
using namespace zmq;
//Function prototypes

void callPython()
{
	cout<<"Calling the KalmanVisual GUI"<<endl;
	std::string command = "path=$(which uKalmanVisual) \n script=\"GUI.py\" pythonfile=$path$script \n python3 $pythonfile";
	system(command.c_str());
}


//---------------------------------------------------------
// Constructor

KalmanVisual::KalmanVisual()
{
	
 context = new zmq::context_t(1);
 pythonthread = new thread(callPython);	
 cout<<"Construtor"<<endl;	
  m_iterations = 0;
  m_timewarp   = 1;
  t0 = MOOSTime();
  t = 0;
  pk0 = 0;
  pk1 =0;
  pk2=0;
  pk3=0;
  pk4=0;
  pk5=0;
  pk6=0;
  pk7=0;
  xerro0=0;
  xerro1=0;
  xerro2=0;
  xerro3=0;
  xerro4=0;
  xerro5=0;
  xerro6=0;
  xerro7=0;
}

//---------------------------------------------------------
// Destructor

KalmanVisual::~KalmanVisual()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool KalmanVisual::OnNewMail(MOOSMSG_LIST &NewMail)
{
    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++) 
	{
	    CMOOSMsg &msg = *p;
		string key = msg.GetKey();//Nome da mensagem
	    if(key == "PK[0]")
	  	{
	  		pk0 = msg.GetDouble();
			t = MOOSTime();
	  	}
	    if(key == "PK[1]")
	  	{
	  		pk1 = msg.GetDouble();
	  	}
	    if(key == "PK[2]")
	  	{
	  		pk2 = msg.GetDouble();
	  	}
	    if(key == "PK[3]")
	  	{
	  		pk3 = msg.GetDouble();
	  	}
	    if(key == "PK[4]")
	  	{
	  		pk4 = msg.GetDouble();
	  	}
	    if(key == "PK[5]")
	  	{
	  		pk5 = msg.GetDouble();
	  	}
	    if(key == "PK[6]")
	  	{
	  		pk6 = msg.GetDouble();
	  	}
	    if(key == "PK[7]")
	  	{
	  		pk7 = msg.GetDouble();
	  	}
	    if(key == "X_ERRO[0]")
	  	{
	  		xerro0 = msg.GetDouble();
	  	}
	    if(key == "X_ERRO[1]")
	  	{
	  		xerro1 = msg.GetDouble();
	  	}
	    if(key == "X_ERRO[2]")
	  	{
	  		xerro2 = msg.GetDouble();
	  	}
	    if(key == "X_ERRO[3]")
	  	{
	  		xerro3 = msg.GetDouble();
	  	}
	    if(key == "X_ERRO[4]")
	  	{
	  		xerro4 = msg.GetDouble();
	  	}
	    if(key == "X_ERRO[5]")
	  	{
	  		xerro5 = msg.GetDouble();
	  	}
	    if(key == "X_ERRO[6]")
	  	{
	  		xerro6 = msg.GetDouble();
	  	}
	    if(key == "X_ERRO[7]")
	  	{
	  		xerro7 = msg.GetDouble();
	  	}
	 }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool KalmanVisual::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool KalmanVisual::Iterate()
{
	//cout<<"Iterate"<<endl;
	
	m_iterations++;
    if(m_iterations!=0)
    {
		socket_t socket(*context, ZMQ_PAIR);
		socket.bind("ipc:///tmp/mysocket");
		//cout<<"Enviando os dados"<<endl;
	    char buff[1000];
	    sprintf(buff, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", pk0, pk1, pk2, pk3, pk4, pk5, pk6, pk7, xerro0, xerro1, xerro2, xerro3, xerro4, xerro5, xerro6,  xerro7, t-t0);
	    std::string sendString = buff;
		s_send (socket, sendString);
		m_Comms.Notify("VISUALOK", m_iterations);//m
	}
	
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool KalmanVisual::OnStartUp()
{	
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  m_timewarp = GetMOOSTimeWarp();

  RegisterVariables();	
 

  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void KalmanVisual::RegisterVariables()
{
	m_Comms.Register("X_ERRO[0]", 0);
	m_Comms.Register("X_ERRO[1]", 0);
	m_Comms.Register("X_ERRO[2]", 0);
	m_Comms.Register("X_ERRO[3]", 0);
	m_Comms.Register("X_ERRO[4]", 0);
	m_Comms.Register("X_ERRO[5]", 0);
	m_Comms.Register("X_ERRO[6]", 0);
	m_Comms.Register("X_ERRO[7]", 0);
	
	m_Comms.Register("PK[0]", 0);
	m_Comms.Register("PK[1]", 0);
	m_Comms.Register("PK[2]", 0);
	m_Comms.Register("PK[3]", 0);
	m_Comms.Register("PK[4]", 0);
	m_Comms.Register("PK[5]", 0);
	m_Comms.Register("PK[6]", 0);
	m_Comms.Register("PK[7]", 0);
}