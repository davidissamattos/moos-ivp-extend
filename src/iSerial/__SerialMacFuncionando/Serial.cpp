/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: ITA                                            */
/*    FILE: Serial.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Serial.h"
//#include "SerialPort.h"//inclui a classe SerialPort que eu criei

using namespace std;

//---------------------------------------------------------
// Constructor

Serial::Serial()
{
  m_iterations = 0;
  m_timewarp   = 1;
}

//---------------------------------------------------------
// Destructor

Serial::~Serial()
{
//Fechando a conexao com a porta serial
	m_MySerial.SerialClose();
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Serial::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++)
  {
    CMOOSMsg &msg = *p;
//Nome da mensagem de entrada (ainda esta por definir como sera essa mensagem)
    string key = msg.GetKey();
//Pega os valores de M1 e M2	
	if(key == m_m1_name)
	{
		m_m1 = msg.GetDouble();
	}
    if(key == m_m2_name)
	{
		m_m2 = msg.GetDouble();
	}
  }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Serial::OnConnectToServer()
{	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Serial::Iterate()
{
  m_iterations++;
  if(m_iterations!=0)
  {
	//Criacao da string de saida
	  //Trocando motor esquerdo com direito
	serialout_string="";
	serialout_string += "M1=";
  if(m_m1==0)
  {
  	serialout_string += "D";
  }
 else if(m_m1==-1)
  {
  	serialout_string += "T";
  }
 else if(m_m1==1)
  {
  	serialout_string += "F";
  }
	serialout_string += ",M2=";
  if(m_m2==0)
  {
  	serialout_string += "D";
  }
  else if(m_m2==-1)
  {
  	serialout_string += "T";
  }
  else if(m_m2==1)
  {
  	serialout_string += "F";
  }
  serialout_string += "\n";
	
    
	//Debug serial out
	//m_Comms.Notify("SERIAL_OUT",serialout_string);

	if(m_MySerial.IsOpen())
	{
		leituraserial = m_MySerial.SerialRead();
		//m_Comms.Notify("LEITURA_SERIAL",leituraserial);
		//cout<<leituraserial;
		//Processamento dos dados
		//GPS_LAT=xxxxxxx,GPS_LON
		if(leituraserial.substr(0,3)=="GPS")
		{
			size_t pos_init=0;
			size_t pos_end=0;		
			
			string comparestr;
			unsigned long len;
			
			//Latitude
			comparestr = "GPS_LAT=";
			len = comparestr.length();
			
			pos_init=leituraserial.find("GPS_LAT=",0);
			pos_end=leituraserial.find(",GPS_LON=",0);
			temp = leituraserial.substr(pos_init+len,pos_end - (pos_init+len));
			mylat = atof(temp.c_str());
			m_Comms.Notify("NAV_LAT",mylat);
			//Longitude
			pos_init=pos_end;
			pos_end=0;
			comparestr = ",GPS_LON=";
			len = comparestr.length();
			pos_end=leituraserial.find(",GPS_TIME=",0);
			temp =leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
			mylon = atof(temp.c_str());
			m_Comms.Notify("NAV_LONG",mylon);
			//Tempo
			pos_init=pos_end;
			pos_end=0;
			comparestr = ",GPS_TIME=";
			len = comparestr.length();
			pos_end=leituraserial.find(",GPS_SPEED=",0);
			temp=leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
			mytime = atof(temp.c_str());
			m_Comms.Notify("NAV_TIME",mytime);
			//Encontra a velocidade
			pos_init=pos_end;
			pos_end=0;
			comparestr = ",GPS_SPEED=";
			len = comparestr.length();
			pos_end=leituraserial.find(";",0);
			temp=leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
			myspeed = atof(temp.c_str());
			if(myspeed<0) myspeed=0;
			m_Comms.Notify("GPS_SPEED",myspeed);
			
		}
		//Se os dados recebidos sao Da IMU
		if(leituraserial.substr(0,3)=="IMU")
		{
			//String da bussola
			if(leituraserial.substr(0,11)=="IMU_HEADING")
			{
				size_t pos_init=0;
				size_t pos_end=0;
				string comparestr;
				unsigned long len;
				//Heading
				comparestr = "IMU_HEADING=";
				len = comparestr.length();
				pos_init=leituraserial.find("IMU_HEADING=",0);
				pos_end=leituraserial.find(";",0);
				temp = leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
				myheading = atof(temp.c_str());
				m_Comms.Notify("IMU_HEADING",myheading);
			}
			//String do acelerometro
			if(leituraserial.substr(0,6)=="IMU_AX")
			{
				size_t pos_init=0;
				size_t pos_end=0;
				string comparestr;
				unsigned long len;	
				//AX
				comparestr = "IMU_AX=";
				len = comparestr.length();
				pos_init=leituraserial.find("IMU_AX=",0);
				pos_end=leituraserial.find(",IMU_AY=",0);
				temp = leituraserial.substr(pos_init+len,pos_end - (pos_init+len));
				double ax = atof(temp.c_str());
				m_Comms.Notify("IMU_AX",ax);
				//AY
				pos_init=pos_end;
				pos_end=0;
				comparestr = ",IMU_AY=";
				len = comparestr.length();
				pos_end=leituraserial.find(",IMU_AZ=",0);
				temp =leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
				double ay = atof(temp.c_str());
				m_Comms.Notify("IMU_AY",ay);
				//AZ
				pos_init=pos_end;
				pos_end=0;
				comparestr = ",IMU_AZ=";
				len = comparestr.length();
				pos_end=leituraserial.find(",IMU_ATIME=",0);
				temp =leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
				double az = atof(temp.c_str());
				m_Comms.Notify("IMU_AZ",az);
				//ATIME
				pos_init=pos_end;
				pos_end=0;
				comparestr = ",IMU_ATIME=";
				len = comparestr.length();
				pos_end=leituraserial.find(";",0);
				temp =leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
				double atime = atof(temp.c_str());
				m_Comms.Notify("IMU_ATIME",atime);
				
			}
			//String do giroscopio
			if(leituraserial.substr(0,6)=="IMU_WX")
			{
				size_t pos_init=0;
				size_t pos_end=0;
				string comparestr;
				unsigned long len;	
				//WX
				comparestr = "IMU_WX=";
				len = comparestr.length();
				pos_init=leituraserial.find("IMU_WX=",0);
				pos_end=leituraserial.find(",IMU_WY=",0);
				temp = leituraserial.substr(pos_init+len,pos_end - (pos_init+len));
				double wx = atof(temp.c_str());
				m_Comms.Notify("IMU_WX",wx);
				//WY
				pos_init=pos_end;
				pos_end=0;
				comparestr = ",IMU_WY=";
				len = comparestr.length();
				pos_end=leituraserial.find(",IMU_WZ=",0);
				temp =leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
				double wy = atof(temp.c_str());
				m_Comms.Notify("IMU_WY",wy);
				//WZ
				pos_init=pos_end;
				pos_end=0;
				comparestr = ",IMU_WZ=";
				len = comparestr.length();
				pos_end=leituraserial.find(",IMU_WTIME=",0);
				temp =leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
				double wz = atof(temp.c_str());
				m_Comms.Notify("IMU_WZ",wz);
				//WTIME
				pos_init=pos_end;
				pos_end=0;
				comparestr = ",IMU_WTIME=";
				len = comparestr.length();
				pos_end=leituraserial.find(";",0);
				temp =leituraserial.substr(pos_init+len,pos_end- (pos_init+len));
				double wtime = atof(temp.c_str());
				m_Comms.Notify("IMU_WTIME",wtime);
			}
			
			
		}
		//escreve os dados dos motores para o arduino
		m_MySerial.SerialWrite(serialout_string);
	}
		m_Comms.Notify("SERIAL_OPEN",m_MySerial.IsOpen());
		//m_Comms.Notify("BaudRate",m_BaudRate);
		m_Comms.Notify("BaudRateSETADA",m_MySerial.whichBaud(3));
  }
return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Serial::OnStartUp()
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
      	if(param == "SERIAL_PORT")
		{
 	       		m_portDirection = value;
		}		
		else if(param == "M1")
		{
 	       		m_m1_name = value;
		}
		else if(param == "M2")
		{
 	       		m_m2_name = value;
		}
		else if(param == "BAUDRATE")
		{
 	       		m_BaudRate = atoi(value.c_str());//converte a string em inteiro
		}
		else if(param == "OPTIONS")
		{
 	       		m_serial_options =value;
		}
	}
   }
//inicializacao de variaveis
   m_iterations=0;
   m_timewarp = GetMOOSTimeWarp();

//inicializacao da porta serial
	m_MySerial.SerialChangePort(m_portDirection);//inicializa a serial com uma porta
	m_MySerial.SerialBegin(m_BaudRate);
	
	if(m_serial_options=="ARDUINO")
		{
			m_MySerial.SetOptions(1);
		}
	if(m_serial_options=="GPS")
		{
			m_MySerial.SetOptions(2);
		}
  	sleep(2);
//utiliza a funcao de teste do GPS
	//ConfigGPS();

	RegisterVariables();	
  	return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Serial::RegisterVariables()
{
	m_Comms.Register(m_m1_name, 0);
	m_Comms.Register(m_m2_name, 0);
//Registrando para a variavel dada pelo processo de configuracao com uma frequencia de 1Hz. No caso eu irei fazer um uPokeDB com a variavel SEQ
}

