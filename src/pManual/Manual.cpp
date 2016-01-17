/************************************************************/
/*    NAME: David Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: Manual.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Manual.h"

using namespace std;
const int LeftAxixXThreshold = 16000;
const int LeftAxixYThreshold = 16000;
const double Pi = 3.1415692;
//---------------------------------------------------------
// Constructor

Manual::Manual()
{
  m_iterations = 0;
  m_timewarp   = 1;
  keyboardInstructions();
  manualControl = "true";
  m_Comms.Notify("MANUAL_CONTROL","TRUE");
  if (SDL_Init(SDL_INIT_JOYSTICK ) < 0)
  {
	  fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
	  cout << "No joystick support will be available" <<endl;
  }
  connectJoystick();
  
  keyboardThread = new thread(&Manual::keyboardControl,this);
  joystickThread = new thread(&Manual::joystickControl,this);	

}

//---------------------------------------------------------
// Destructor

Manual::~Manual()
{
	cout << "Exiting....";
	//cout << "Exiting....";
	disconnectJoystick();
	SDL_Quit();

}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Manual::OnNewMail(MOOSMSG_LIST &NewMail)
{
	MOOSMSG_LIST::iterator p; 
	for(p=NewMail.begin(); p!=NewMail.end(); p++) 
	{
		string temp;
		CMOOSMsg &msg = *p;
		string key = msg.GetKey();//Nome da mensagem
		//Pega os valores das variaveis
		if(key == "MANUAL_CONTROL")
		{
			manualControl = msg.GetString();
			toLower(manualControl);
			if(manualControl=="true") // realiza o controle
			{
				//Configuracoes para parar o pHelm
				m_Comms.Notify("MOOS_MANUAL_OVERIDE","TRUE");
				m_Comms.Notify("MOOS_MANUAL_OVERRIDE","TRUE");
				//Desativar o controle do barco baseado no pHelm
				m_Comms.Notify("DISABLE_CONTROLEBARCO","true");
				//Turn off engine
				double M1=0;
				double M2=0;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			if(manualControl=="false") 
			{
				//Configuracoes para voltar o pHelm
				m_Comms.Notify("MOOS_MANUAL_OVERIDE","FALSE");
				m_Comms.Notify("MOOS_MANUAL_OVERRIDE","FALSE");
				//Desativar o controle do barco baseado no pHelm
				m_Comms.Notify("DISABLE_CONTROLEBARCO","false");
			}
		
		}
	}	
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Manual::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Manual::Iterate()
{

	//Not used -> the functions are being called in threads
   return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Manual::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
	  
	  //Controles que estarao na configuracao do aplicativo
      //if(param == "CONTROL_TYPE") 
	  //{
		//   toLower(value);
		  // control_type = value;
      //}
    }
  }
  
  m_timewarp = GetMOOSTimeWarp();

  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Manual::RegisterVariables()
{
  m_Comms.Register("MANUAL_CONTROL", 0);
}


//Instructions
void Manual::keyboardInstructions()
{
	cout<< "**** Instructions for using pManual ****"<<endl;
	cout<<"Control command:"<<endl;
	cout<<"q	w	e 		↖	↑  	↗"<<endl;
	cout<<"a	s	d		← 	o	→"<<endl;
	cout<<"z	x	c		↙	↓	↘"<<endl<<endl;
	
	
	cout<<"Other commands:"<<endl;
	cout<<"j: Connect Joystick"<<endl;
	cout<<"u: Disconnect Joystick"<<endl;
	cout<<"n: Navigate (Return to pHelm)"<<endl;
	cout<<"m: Start Manual control"<<endl;
	cout<<"h: This help"<<endl;
	
	
	cout<<"PS: Press twice the first manual navigation key after 'm' command"<<endl;
				//		q	w	e
				//		a	s	d
				//		z	x	c
				//
				// Equivale a (M1,M2)
				//	()		(1,1)		()
				//	()		(0,0)		()
				//	()		(-1,-1)		()
				//
}

void Manual::keyboardControl()
{
	while(true)
	{
		char myinput = myMOOSGetch();
		//Pegar uma leitura do teclado
		double M1;
		double M2;
		switch(myinput)
		{
			case 'j':
			connectJoystick();
			break;
		
			case 'u':
			disconnectJoystick();
			break;
		
			case 'w':
			if(manualControl=="true") // realiza o controle
			{
				M1= 1;
				M2 = 1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			break;
		
			case 's':
			if(manualControl=="true") // realiza o controle
			{
				M1= 0;
				M2 = 0;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			break;
		
			case 'x':
			if(manualControl=="true") // realiza o controle
			{
				M1= -1;
				M2 = -1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			break;
		
			case 'a':
			if(manualControl=="true") // realiza o controle
			{
				M1= -1;
				M2 = 1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			break;
		
			case 'd':
			if(manualControl=="true") // realiza o controle
			{
				M1= 1;
				M2 = -1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			break;
		
			case 'q':
			if(manualControl=="true") // realiza o controle
			{
				M1= 0;
				M2 = 1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			break;
		
			case 'e':
			if(manualControl=="true") // realiza o controle
			{
				M1= 1;
				M2 = 0;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			break;
		
			case 'z':
			if(manualControl=="true") // realiza o controle
			{
				M1= -1;
				M2 = 0;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			break;
		
			case 'c':
			if(manualControl=="true") // realiza o controle
			{
				M1= 0;
				M2 = -1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			break;
		
			case 'n':
				//Configuracoes para parar o pHelm
				//m_Comms.Notify("MOOS_MANUAL_OVERIDE","FALSE");
				m_Comms.Notify("MANUAL_CONTROL","FALSE"); manualControl = "false";
				m_Comms.Notify("MOOS_MANUAL_OVERRIDE","FALSE");
				//Desativar o controle do barco baseado no pHelm
				m_Comms.Notify("DISABLE_CONTROLEBARCO","false");
			break;
		
			case 'h':
				keyboardInstructions();
			break;
		
			case 'm':
			//Configuracoes para parar o pHelm
				//m_Comms.Notify("MOOS_MANUAL_OVERIDE","TRUE");
				m_Comms.Notify("MANUAL_CONTROL","TRUE");manualControl = "true";
				m_Comms.Notify("MOOS_MANUAL_OVERRIDE","TRUE");
				//Desativar o controle do barco baseado no pHelm
				m_Comms.Notify("DISABLE_CONTROLEBARCO","true");
				M1= 0;
				M2 = 0;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			break;
		}	
	}	
}

void Manual::joystickControl()
{
	//cout<<"joy"<<endl;
	//cout<<joystick<<endl;
	while(true)
	{
	    if (joystick != NULL)
	    {
		
			//ocorreu um evento e no joystick 0
	         while( SDL_PollEvent( &event ) != 0 &&  event.jaxis.which == 0)
			 {
				 //Eventos dos Joysticks
				 if( event.type == SDL_JOYAXISMOTION )
	             {
					 //Navegacao Left Joystick
					 if( event.jaxis.axis == LeftXAxis )
					 	LeftXAxis_value = event.jaxis.value;
					 if( event.jaxis.axis == LeftYAxis )
					 	LeftYAxis_value = event.jaxis.value;

				 
					 //Future Camera - Right Joystick
					 if( event.jaxis.axis == RightXAxis )
					 {
					 	RightXAxis_value = event.jaxis.value;
					 }
					 if( event.jaxis.axis == RightYAxis )
					 {
					 	RightYAxis_value = event.jaxis.value;
					 }
				 
					 //Triggers
					 if( event.jaxis.axis == LeftTrigger )
					 {
					 	LeftTrigger_value = event.jaxis.value;
					 }
					 if( event.jaxis.axis == RightTrigger )
					 {
					 	RightTrigger_value = event.jaxis.value;
					 }
				 
				 
					 //Process Navigation data
					 joystickNavigation();
				 
	             }
			 
				 //Eventos dos botoes
				 if( event.type == SDL_JOYBUTTONDOWN )
		         {
					 //Start manual control
					 if( event.jbutton.button == StartButton )
			         {
			 		//Configuracoes para parar o pHelm
			 			//m_Comms.Notify("MOOS_MANUAL_OVERIDE","TRUE");
			 			m_Comms.Notify("MANUAL_CONTROL","TRUE");manualControl = "true";
			 			m_Comms.Notify("MOOS_MANUAL_OVERRIDE","TRUE");
			 			//Desativar o controle do barco baseado no pHelm
			 			m_Comms.Notify("DISABLE_CONTROLEBARCO","true");
			 			double M1= 0;
			 			double M2 = 0;
			 			m_Comms.Notify("M1",M1);
			 			m_Comms.Notify("M2",M2);
					 }
					 //Stop manual control
					 else if( event.jbutton.button == BackButton )
			         {
			 			//Configuracoes para parar o pHelm
			 			//m_Comms.Notify("MOOS_MANUAL_OVERIDE","FALSE");
			 			m_Comms.Notify("MANUAL_CONTROL","FALSE"); manualControl = "false";
			 			m_Comms.Notify("MOOS_MANUAL_OVERRIDE","FALSE");
			 			//Desativar o controle do barco baseado no pHelm
			 			m_Comms.Notify("DISABLE_CONTROLEBARCO","false");
					 }
				 
				 
				 }
			 
				 //Eventos dos Hats
		         if( event.type == SDL_JOYHATMOTION )
		         {

		         }
			 }
	    }
	}
	
}

void Manual::connectJoystick()
{
    if( SDL_NumJoysticks() < 1 )
    {
      cout<<"Warning: No joysticks connected!"<<endl;
  	  cout<<"Connect a XBox360 controller and press J again"<<endl;
    }
    else
    {
  	    SDL_JoystickEventState(SDL_ENABLE);
        joystick = SDL_JoystickOpen(0);
        if (joystick == NULL)
        {
            cout << "Error openning joystick";
        }
		else
		{
			 cout<<endl;
			 cout << "Connected: " << SDL_JoystickName(joystick) <<endl;
			 cout<<endl;
		}
    }
}

void Manual::disconnectJoystick()
{
	if(joystick != NULL)
		SDL_JoystickClose(joystick);
}

void Manual::joystickNavigation()
{
	if(manualControl == "true")
	{
		double M1;
		double M2;
		double joystickAngle = atan2( -LeftYAxis_value, LeftXAxis_value ) * ( 180.0 / Pi);
		//Se um dos 2 forem maiores que o limite ligam-se os motores
		if(abs(LeftYAxis_value)>LeftAxixYThreshold || abs(LeftXAxis_value)>LeftAxixXThreshold)
		{
			if (joystickAngle >= 110 && joystickAngle < 160)
			{
				M1= 0;
				M2 = 1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			if (joystickAngle >= 70 && joystickAngle < 110)
			{
				M1= 1;
				M2 = 1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			if (joystickAngle >= 20 && joystickAngle < 70)
			{
				M1= 1;
				M2 = 0;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			if (joystickAngle >= -20 && joystickAngle < 20)
			{
				M1= 1;
				M2 = -1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			if (joystickAngle >= -70 && joystickAngle < -20)
			{
				M1= -1;
				M2 = 0;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			if (joystickAngle >= -110 && joystickAngle < -70)
			{
				M1= -1;
				M2 = -1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			if (joystickAngle >= -160 && joystickAngle < -110)
			{
				M1= 0;
				M2 = -1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
			if ((joystickAngle >= 160 && joystickAngle <= 180) || (joystickAngle <= -180 && joystickAngle > -160))
			{
				M1= -1;
				M2 = 1;
				m_Comms.Notify("M1",M1);
				m_Comms.Notify("M2",M2);
			}
		}
		//Caso contrario desligam-se os motores
		else
		{
			M1= 0;
			M2 = 0;
			m_Comms.Notify("M1",M1);
			m_Comms.Notify("M2",M2);
		}
	}	
}



///// Funcao para pegar o conteudo do terminal sem ter que apertar enter

int myMOOSGetch()
{
    int c, fd=0;
    struct termios term, oterm;

    /* get the terminal settings */
    tcgetattr(fd, &oterm);

    /* get a copy of the settings, which we modify */
    memcpy(&term, &oterm, sizeof(term));

    /* put the terminal in non-canonical mode, any
    reads will wait until a character has been
    pressed. This function will not time out */
    term.c_lflag = term.c_lflag & (!ICANON);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSANOW, &term);

    /* get a character. c is the character */
	//hold all activities
    c=getchar();

    /* reset the terminal to its original state */
    tcsetattr(fd, TCSANOW, &oterm);

    /* return the charcter */
    return c;

}

//To Upper e To Lower functions
void toUpper(basic_string<char>& s) {
   for (std::basic_string<char>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = toupper(*p); // toupper is for char
   }
}

void toUpper(basic_string<wchar_t>& s) {
   for (std::basic_string<wchar_t>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = towupper(*p); // towupper is for wchar_t
   }
}

void toLower(basic_string<char>& s) {
   for (std::basic_string<char>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = tolower(*p);
   }
}

void toLower(basic_string<wchar_t>& s) {
   for (std::basic_string<wchar_t>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = towlower(*p);
   }
}
