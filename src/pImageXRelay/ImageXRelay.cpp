/************************************************************/
/*    NAME: David Mattos                                    */
/*    ORGN: ITA                                            */
/*    FILE: ImageXRelay.cpp                                 */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ImageXRelay.h"

using namespace std;

//---------------------------------------------------------
// Constructor

ImageXRelay::ImageXRelay()
{
  m_iterations = 0;
  m_timewarp   = 1;
}

//---------------------------------------------------------
// Destructor

ImageXRelay::~ImageXRelay()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool ImageXRelay::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++)
   {
    CMOOSMsg &msg = *p;

    string key   = msg.GetKey();
    if(key == m_incoming_var) 
      m_tally_recd++;
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool ImageXRelay::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool ImageXRelay::Iterate()
{
  m_iterations++;
  if(m_iterations!=0)
  {
	  Mat image_;
	  image_ = CaptureWebcam();
	  //saveJPG("foto",image_,70);
	 // m_Comms.Notify("CONTAGEM", m_tally_recd);
	  Notify("IMAGEM",(void*)image_.data,image_.size().area(),MOOSLocalTime());
	 // m_Comms.Notify("IMAGEM", image);
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool ImageXRelay::OnStartUp()
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
      
      if(param == "IMAGE")
	  {
		  m_incoming_var = value;
      }
    }
  }
  
  m_timewarp = GetMOOSTimeWarp();

  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void ImageXRelay::RegisterVariables()
{
    if(m_incoming_var != "")
      m_Comms.Register(m_incoming_var, 0.2);
}

//-----------------

bool ImageXRelay::saveJPG(string name, Mat frame, int quality)
{
	//Save image
	vector<int> compression_params; 
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY); 
	compression_params.push_back(quality); //specify the compression quality
	bool bSuccess = imwrite(name+".jpg", frame, compression_params);
	return bSuccess;
}

Mat ImageXRelay::CaptureWebcam()
{
    //Inicio da captura
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
    Mat frame;
	//Verificacao se abriu corretamente a camera
	if(!cap.isOpened())
	{
		cout << "No camera detected" << endl;
	}
	cap >> frame;
	//Mat bw_image_;
	//cv::cvtColor(frame, bw_image_, COLOR_BGR2GRAY);
	//cv::resize(bw_image_, frame, frame.size(), 0, 0, cv::INTER_NEAREST);
	//cv::resize(frame, frame, frame.size(), 0, 0, cv::INTER_NEAREST);
	//cap.release();
	return frame;
}

