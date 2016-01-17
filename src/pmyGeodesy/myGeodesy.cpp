/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: ITA                                             */
/*    FILE: myGeodesy.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "myGeodesy.h"

#define degree2rad 0.01745327778

using namespace std;

//---------------------------------------------------------
// Constructor

myGeodesy::myGeodesy()
{
  m_iterations = 0;
  m_timewarp   = 1;
  a=6378137;//m
  b=6356752.314245;//m
  f=(a-b)/a;
  e=sqrt(f*(2-f));
  
}

//---------------------------------------------------------
// Destructor

myGeodesy::~myGeodesy()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool myGeodesy::OnNewMail(MOOSMSG_LIST &NewMail)
{
	MOOSMSG_LIST::iterator p; 
	for(p=NewMail.begin(); p!=NewMail.end(); p++) 
	{
		string temp;
		CMOOSMsg &msg = *p;
		string key = msg.GetKey();//Nome da mensagem
		//Pega os valores das variaveis
		if(key == "NAV_LAT")
		{
			mylat = msg.GetDouble();
			//temp = msg.GetString();
			//cout<<"mylat :"<<temp<<endl;
			//mylat = std::stod (temp.c_str());
			cout<<"mylat :"<<mylat<<endl;
		}
		if(key == "NAV_LONG")
		{
			mylon = msg.GetDouble();
			//temp = msg.GetString();
			cout<<"mylon :"<<temp<<endl;
			//mylon = std::stod (temp.c_str());
			std::cout << std::fixed;
			cout<<std::setprecision(8)<<"mylon: "<<mylon<<endl;
		}
		if(key == "IMU_HEADING")
		{
			myheading = msg.GetDouble();
		}
		if(key == "GPS_SPEED")
		{
			myspeed = msg.GetDouble();
		}
	}	
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool myGeodesy::OnConnectToServer()
{	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool myGeodesy::Iterate()
{
   // happens AppTick times per second
	m_iterations++;
	Global2Local();
	if(north > 10000 || north < -10000 || east > 10000 || east < -10000)
	{
		north=0;
		east=0;
		m_Comms.Notify("INFO_GEODESY","ERROR");
	}
	else
	{
		m_Comms.Notify("NAV_X",east);
		m_Comms.Notify("NAV_Y",north);
		m_Comms.Notify("INFO_GEODESY","OK");
		//Parametros que convertem para os nomes corretos sem o filtro de kalman
		m_Comms.Notify("NAV_HEADING",myheading);
		m_Comms.Notify("NAV_SPEED",myspeed);
		
	}	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool myGeodesy::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams))
  {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "LAT_ORIGIN") 
	  {
		  std::string valor = value;
		  lat_origin = std::stod (valor.c_str());
		  cout<<"LAT_ORIGEM :"<<lat_origin<<endl;
      }
      else if(param == "LON_ORIGIN")
	   {
		  std::string valor = value;
		  lon_origin = std::stod (valor.c_str());
		  cout<<"LON_ORIGEM :"<<lon_origin<<endl;
      }
      else if(param == "ALTITUDE")
	   {
		  std::string valor = value;
		  altitude = std::stod(valor.c_str());
      }
    }
  }
  
  m_timewarp = GetMOOSTimeWarp();


	double N;
	N = a/(sqrt(1-e*e*sin(lat_origin*degree2rad)*sin(lat_origin*degree2rad)));
	origin_x = (N+altitude)*cos(lat_origin*degree2rad)*cos(lon_origin*degree2rad);
	origin_y = (N+altitude)*cos(lat_origin*degree2rad)*sin(lon_origin*degree2rad);
	origin_z = (N*(1-e*e)+altitude)*sin(lat_origin*degree2rad);
	
	m_Comms.Notify("LAT_ORIGIN",lat_origin);
	m_Comms.Notify("ALTITUDE",altitude);
	m_Comms.Notify("LON_ORIGIN",lon_origin);


  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void myGeodesy::RegisterVariables()
{
	m_Comms.Register("NAV_LAT", 0);
	m_Comms.Register("NAV_LONG", 0);
	m_Comms.Register("IMU_HEADING", 0);
	m_Comms.Register("GPS_SPEED", 0);
}

//---------------------------------------------------------

void myGeodesy::Global2Local()
{
	//Usando o elipsoide WGS84
	double N;
	N = a/(sqrt(1-e*e*sin(mylat*degree2rad)*sin(mylat*degree2rad)));

	double geo_x;
	double geo_y;
	double geo_z;

	geo_x = (N+altitude)*cos(mylat*degree2rad)*cos(mylon*degree2rad);
	geo_y = (N+altitude)*cos(mylat*degree2rad)*sin(mylon*degree2rad);
	geo_z = (N*(1-e*e)+altitude)*sin(mylat*degree2rad);
	
	//cout<<"X -X0 :"<<geo_x-origin_x<<endl;
	//cout<<"Y -Y0 :"<<geo_y-origin_y<<endl;
	
	north = (geo_z - origin_z)*cos(mylat*degree2rad) - (geo_x - origin_x)*cos(mylon*degree2rad)*sin(mylat*degree2rad)-(geo_y - origin_y)*sin(mylon*degree2rad)*sin(mylat*degree2rad);
	cout<<"North :"<<north<<endl;
	
	east = (geo_y - origin_y)*cos(mylon*degree2rad) - (geo_x - origin_x)*sin(mylon*degree2rad);
	cout<<"East :"<<east<<endl;
	
	down = -(geo_x - origin_x)*cos(mylat*degree2rad)*cos(mylon*degree2rad) - (geo_z - origin_z)*sin(mylat*degree2rad) - (geo_y - origin_y)*cos(mylat*degree2rad)*sin(mylon*degree2rad);
	
}