/************************************************************/
/*    NAME: David Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: Manual.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Manual_HEADER
#define Manual_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <iostream>     // std::cout, std::fixed
#include <stdlib.h>     /* atof */
#include <cstring>
#include <algorithm>
#include <cctype>
#include <cwctype>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <cmath>
#include <thread>

void toUpper(std::basic_string<char>& s);
void toUpper(std::basic_string<wchar_t>& s);
void toLower(std::basic_string<char>& s);
void toLower(std::basic_string<wchar_t>& s);
int myMOOSGetch();






class Manual : public CMOOSApp
{
 public:
   Manual();
   ~Manual();

 protected:
   //MOOS
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
   //Keyboard
   void keyboardControl();
   void keyboardInstructions();
   //Joystick
   void joystickControl();
   void connectJoystick();
   void disconnectJoystick();
   void joystickNavigation();
   
   std::string manualControl;
   std::string control_type;

 private: 
	 std::thread* keyboardThread;
	 std::thread* joystickThread;
	 
	 // Configuration variables
 	 SDL_Joystick *joystick;
     //Event handler
     SDL_Event event;
	 //Joystick parameter
	 double LeftXAxis_value;
	 double LeftYAxis_value;
	 double RightXAxis_value;
	 double RightYAxis_value;
	 double RightTrigger_value;
	 double LeftTrigger_value;
	 
	 
	 
     enum JoystickAxis
     {
         LeftXAxis = 0,
         LeftYAxis = 1,
         LeftTrigger = 2,
         RightXAxis = 3,
         RightYAxis = 4,
         RightTrigger = 5,
     };
     enum JoystickButtons
     {
         AButton = 0,
         BButton = 1,
         XButton = 2,
         YButton = 3,
         LBButton = 4,
         RBButton = 5,
         LeftStickButton = 6,
         RightStickButton = 7,
         StartButton = 8,
         BackButton = 9,
         HomeButton = 10,
         UpButton = 11,
         DownButton = 12,
         LeftButton = 13,
         RightButton = 14,
     };

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
