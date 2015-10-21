/************************************************************/
/*    NAME: David                                              */
/*    ORGN: MIT                                             */
/*    FILE: ToApacheJSON.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ToApacheJSON.h"

using namespace std;
using namespace boost::filesystem;

//---------------------------------------------------------
// Constructor

ToApacheJSON::ToApacheJSON()
{
  m_iterations = 0;
  m_timewarp   = 1;
  json_file_name = "";
  community = "";
  start_time = MOOSTime();
}

//---------------------------------------------------------
// Destructor

ToApacheJSON::~ToApacheJSON()
{
	delete jsonpath;
	delete jsonFileWithPath;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool ToApacheJSON::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
	
	if(community == "")community = msg.GetCommunity();
	string key = msg.GetKey();//Messages name
	//Register for all variables available trough *.moos file
	for(std::vector<std::string>::iterator it = variables.begin(); it != variables.end(); ++it) 
	{
		// if the current index is needed:
	    unsigned int i = std::distance(variables.begin(), it); 
		if(key == (*it))
	  	{
			if(msg.IsDouble()) variablesValues[i] = std::to_string(msg.GetDouble());
			if(msg.IsString()) variablesValues[i] = msg.GetString();
			cout << variables[i] << ":" << variablesValues[i]<<endl;
	  	}
		
	}
	
#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool ToApacheJSON::OnConnectToServer()
{
   //As the register variables are defined in the moos file, do nothing here
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool ToApacheJSON::Iterate()
{
	m_iterations++;
	//std::cout<< <<std::endl;
	if(m_iterations>1)
	{
		
		root["MOOSTime"] = MOOSTime()-start_time;
		root["Community"] = community;
		for(std::vector<std::string>::iterator it = variables.begin(); it != variables.end(); ++it) 
		{
			// if the current index is needed:
			unsigned int i = std::distance(variables.begin(), it); 
			root["Variables"][*it] =  variablesValues[i];
		}
		Json::StyledWriter styledWriter;
		file.open(jsonFileWithPath->native());
		file << styledWriter.write(root);
		file.close();
	}
	else
	{
		json_file_name = community+".json";
		jsonFileWithPath = new path(jsonpath->string()+ json_file_name);
		cout<<"File Name with Path " << jsonFileWithPath->native() <<endl;
	}
	
	
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool ToApacheJSON::OnStartUp()
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

			if(param == "VARIABLES")
			{
				std::string str = value;
				std::istringstream ss(str);
				while(std::getline(ss, value, ','))
				{
				    variables.push_back(value);
					cout <<"VARIABLES: "<<value<<endl;
				}
				//Resizing the variablesValue to have the number of elements
				variablesValues.resize(variables.size());
				cout << "Variables has " << variables.size() << " elements"<<endl;
				cout << "VariablesValues has " << variablesValues.size() << " elements"<<endl;
			}

			if(param == "PATH")
			{
				jsonpath = new path(value);
				cout <<"PATH: "<<jsonpath->native()<<endl;
			}
			if(param == "JSON_FILE_NAME")
			{
				json_file_name = value;
			}
		}

		m_timewarp = GetMOOSTimeWarp();
	}
	RegisterVariables();	
	return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void ToApacheJSON::RegisterVariables()
{
	
	//Iterating through the vector to register for the variables
	for(std::vector<std::string>::iterator it = variables.begin(); it != variables.end(); ++it) 
	{
		m_Comms.Register((*it), 0);
		
	}
}

