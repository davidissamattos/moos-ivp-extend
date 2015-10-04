/************************************************************/
/*    NAME: David Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: ImageTransmitt.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ImageTransmitt.h"

using namespace std;

//---------------------------------------------------------
// Constructor

ImageTransmitt::ImageTransmitt()
{
  m_iterations = 0;
  m_timewarp   = 1;
}

//---------------------------------------------------------
// Destructor

ImageTransmitt::~ImageTransmitt()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool ImageTransmitt::OnNewMail(MOOSMSG_LIST &NewMail)
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

bool ImageTransmitt::OnConnectToServer()
{

   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool ImageTransmitt::Iterate()
{
    m_iterations++;
    if(m_iterations!=0)
    {
  	  Mat image_;
  	  image_ = CaptureWebcam();
  	  //saveJPG("foto",image_,70);

	  //Notificacao binaria
  	  Notify("IMAGEM",(void*)image_,image_.size().area(),MOOSLocalTime());
    }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool ImageTransmitt::OnStartUp()
{
	//debug
	namedWindow("transmission",WINDOW_NORMAL);
	
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
      if(param == "CAMERA_NUMBER")
  	  {
		  string temp;
		  temp = value;
		  std::string::size_type sz;   // alias of size_t
		  camera_number = std::stoi(temp,&sz);
		  cout<<"camera number:"<<camera_number<<endl;
      }
	  
    }
    }
  
    m_timewarp = GetMOOSTimeWarp();

    RegisterVariables();	
    return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void ImageTransmitt::RegisterVariables()
{
    if(m_incoming_var != "")
      m_Comms.Register(m_incoming_var, 1);
}






bool ImageTransmitt::saveJPG(string name, Mat frame, int quality)
{
	//Save image
	vector<int> compression_params; 
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY); 
	compression_params.push_back(quality); //specify the compression quality
	bool bSuccess = imwrite(name+".jpg", frame, compression_params);
	return bSuccess;
}

Mat ImageTransmitt::CaptureWebcam()
{
    //Inicio da captura
	cap.open(camera_number);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 360);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	
    Mat frame = cv::Mat(240 ,360 , CV_8UC3);
	//Verificacao se abriu corretamente a camera
	if(!cap.isOpened())
	{
		cout << "No camera detected" << endl;
	}
		cap >> frame;
		//Mat bw_image_;
		//cv::cvtColor(frame, bw_image_, COLOR_BGR2GRAY);
		//cv::resize(bw_image_, interm, bw_image_.size(), 0, 0, cv::INTER_NEAREST);
		cv::resize(frame, interm, frame.size(), 0, 0, cv::INTER_NEAREST);
	

		//imshow("transmission", frame);
		//cap.release();
		return interm;
		//return bw_image_;

}



