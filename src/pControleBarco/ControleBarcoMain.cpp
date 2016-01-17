/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: ITA                                           */
/*    FILE: ControleBarcoMain.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <string>
#include "ControleBarco.h"

using namespace std;

int main(int argc, char *argv[])
{
  // default parameters file
  string sMissionFile = "ControleBarco.moos";
        
  //under what name shoud the application register with the MOOSDB?
  string sMOOSName = "pControleBarco";
  
  switch(argc)
    {
    case 3:
      //command line says don't register with default name              
      sMOOSName = argv[2];
    case 2:
      //command line says don't use default config file
      sMissionFile = argv[1];
    }
  
  //make an application
  ControleBarco ControleBarcoApp;

  //run it
  ControleBarcoApp.Run(sMOOSName.c_str(), sMissionFile.c_str());
  
  return(0);
}

