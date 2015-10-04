/************************************************************/
/*    NAME: David Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: ImageDisplayProcess.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ImageDisplayProcess.h"

using namespace std;

//---------------------------------------------------------
// Constructor

ImageDisplayProcess::ImageDisplayProcess()
{
  m_iterations = 0;
  m_timewarp   = 1;
}

//---------------------------------------------------------
// Destructor

ImageDisplayProcess::~ImageDisplayProcess()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool ImageDisplayProcess::OnNewMail(MOOSMSG_LIST &NewMail)
{
    MOOSMSG_LIST::iterator p;
    
    for(p=NewMail.begin(); p!=NewMail.end(); p++)
     {
      CMOOSMsg &msg = *p;
	  Mat img;
      string key   = msg.GetKey();
	  if(msg.IsName("IMAGEM"))
		   { 
			 imagem = cv::Mat(240 ,360 , CV_8UC3);
			 cout<<"Imagem recebida";
			 std::cerr<<"bytes : "<<msg.GetBinaryDataSize()<<" latency "<<std::setprecision(3)<<((MOOSLocalTime() - msg.GetTime())*1e3)<<" ms/r "<<std::endl;
			 memcpy(imagem,msg.GetBinaryData(),msg.GetBinaryDataSize());
			 
		  }

	}
	
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool ImageDisplayProcess::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool ImageDisplayProcess::Iterate()
{
    m_iterations++;
    if(m_iterations!=0)
    {
		if (!imagem.empty())
		{
		    imshow("Imagem", imagem);
			cv::waitKey(1);
		}
    }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool ImageDisplayProcess::OnStartUp()
{
	//imagem = cv::Mat(360 ,640 , CV_8UC1);
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
      
       // if(param == "IMAGEM")
  	   // {
		//	m_incoming_var = value;
       // }
      }
    }
  
    m_timewarp = GetMOOSTimeWarp();

    RegisterVariables();	
    return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void ImageDisplayProcess::RegisterVariables()
{
	m_Comms.Register("IMAGEM", 1);
}

