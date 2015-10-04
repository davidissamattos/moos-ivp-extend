/************************************************************/
/*    NAME: David Mattos                                              */
/*    ORGN:ITA                                             */
/*    FILE: ImageXRelayDisplay.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ImageXRelayDisplay.h"

using namespace std;

//---------------------------------------------------------
// Constructor

ImageXRelayDisplay::ImageXRelayDisplay()
{
  m_iterations = 0;
  m_timewarp   = 1;
}

//---------------------------------------------------------
// Destructor

ImageXRelayDisplay::~ImageXRelayDisplay()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool ImageXRelayDisplay::OnNewMail(MOOSMSG_LIST &NewMail)
{
    MOOSMSG_LIST::iterator p;
    
    for(p=NewMail.begin(); p!=NewMail.end(); p++)
     {
      CMOOSMsg &msg = *p;

      string key   = msg.GetKey();
      //if(key == "IMAGEM")
	  //{ 
        //imagem = msg.GetBinaryData();
		//image_size=msg.GetBinaryDataSize();
      //}
  	
	  if(msg.IsName("IMAGEM"))
		   { 
			 cout<<"Imagem recebida";
			 std::cerr<<"bytes : "<<msg.GetBinaryDataSize()<<" latency "<<std::setprecision(3)<<((MOOSLocalTime() - msg.GetTime())*1e3)<<" ms/r " ;
	  		 memcpy(imagem.data,msg.GetBinaryData(),msg.GetBinaryDataSize());
		  }

	}
	
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool ImageXRelayDisplay::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool ImageXRelayDisplay::Iterate()
{
    m_iterations++;
    if(m_iterations!=0)
    {
		if (!imagem.empty())
		{
		    imshow("Imagem", imagem);
			cv::waitKey(10);
		}
    }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool ImageXRelayDisplay::OnStartUp()
{
	//imagem = cv::Mat(360 ,640 , CV_8UC1);
	//imagem = cv::Mat(360 ,640 , CV_8UC3);
	namedWindow("Imagem",WINDOW_NORMAL);
	
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
      
        if(param == "IMAGEM")
  	  {
  		 // m_incoming_var = value;
        }
      }
    }
  
    m_timewarp = GetMOOSTimeWarp();

    RegisterVariables();	
    return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void ImageXRelayDisplay::RegisterVariables()
{
	m_Comms.Register("IMAGEM", 0.2);
}

