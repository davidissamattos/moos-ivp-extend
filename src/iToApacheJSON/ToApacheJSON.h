/************************************************************/
/*    NAME: David                                              */
/*    ORGN: MIT                                             */
/*    FILE: ToApacheJSON.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef ToApacheJSON_HEADER
#define ToApacheJSON_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>     /* atof */
#include <math.h>       /* sin */
#include <ctime>
#include <fstream>
#include <sstream> //Passing string through startup
#include "json/json.h"



using namespace std;
using namespace boost::filesystem;

class ToApacheJSON : public CMOOSApp
{
 public:
   ToApacheJSON();
   ~ToApacheJSON();

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
   
   double start_time;
   //Variables to subscribe and post
   std::vector<string> variables;
   std::vector<string> variablesValues;
   path *jsonpath;
   path *jsonFileWithPath;
   std::string community;
   std::string json_file_name;
   //Json specific files
   Json::Value root;
   std::vector<Json::Value> VarValue;
   ofstream file;
   
};

#endif 
