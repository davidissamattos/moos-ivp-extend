/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: MIT                                             */
/*    FILE: KalmanSensorFusion.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "KalmanSensorFusion.h"

using namespace std;
using namespace boost::numeric::odeint;
typedef std::vector< double > state_type;

//---------------------------------------------------------
// Constructor

KalmanSensorFusion::KalmanSensorFusion()
{
	m_iterations = 0;
	m_timewarp   = 1;
	
    for (int nCount=0; nCount < 5; nCount++)
  	           x.push_back(0.0);
	x[4]=pi/2;
    tempo_anterior=0;
    tempo_atual=0;
	
	
	//Constante beta para evitar a divergencia
	KalmanBeta = 1/100;
	//Valores padroes para as variancias
	GPSvar = 1;
	COMPASSvar = (1*pi/180)*(1*pi/180);
	ACCELvar = 0.1;
	GYROvar = 0.01;
	
	//Vetores de estados do Filtro de Kalman
	U_INS.resize(3);U_INS << 0,0,pi/2;
	XK_INS.resize(5); XK_INS << 0,0,0,0,pi/2;
	XK1_INS.resize(5); XK1_INS << 0,0,0,0,pi/2;
	
	X_ERRO.resize(8); X_ERRO << 0,0,0,0,0,0,0,0;
	Y_ERRO.resize(3);Y_ERRO << 0,0,0;//[pxINS pyINS psiINS]

	//Bias inicial estimado e de 0 para os acelerometros e gyro
	BiasK.resize(3); BiasK << 0,0,0;
	BiasK1.resize(3); BiasK1 = BiasK;
	
	//Valores de inicializacao caso nao seja definido na hora
	P0_diag.resize(8); P0_diag << 20, 20, 10, 10, (pi/2)*(pi/2), 25, 25, (pi/2)*(pi/2);
	
	PK.resize(8,8);
	PK1.resize(8,8);
	P0.resize(8,8);
	P0 = P0_diag.asDiagonal();

	PK= P0;
	PK1 = P0;
	
	
	//Medidas e estimativas
	U_IMU.resize(3);U_IMU<<0,0,0;
	compass = 0;
	psi_compass = pi/2;
	GPS.resize(2); GPS << 0,0;
	
	//IMU
	VarIMU.resize(3); VarIMU << ACCELvar, ACCELvar, GYROvar;//Variancia [Axb Ayb Wz]
	QK = VarIMU.asDiagonal();
	
	//GPS e bussola
	//Na pratica sao os valores medidos
	VarCOMP.resize(1); VarCOMP << COMPASSvar;//Variancia da Bussola
	VarGPS.resize(2); VarGPS << GPSvar,GPSvar;//Variancia do GPS	
	RK.resize(3,3);
	RK << VarGPS[0], 	0,	 	0,
			0,		VarGPS[1],	0,
			0,			0,		VarCOMP[0];
	
	
	
	//Matrizes consantes do Espaco de estados
	C_ERROS.resize(3,8);
	C_ERROS_T.resize(8,3);
	C_ERROS << 0, 0, 1, 0, 0, 0, 0,	0,
			   0, 0, 0, 1, 0, 0, 0, 0,
			   0, 0, 0, 0, 1, 0, 0,	0;
	
   	C_ERROS_T = C_ERROS.transpose();	
	I8 = Matrix<double, 8, 8>::Identity();

	//Constante da Discretizacao
	T = 0.25;
	ic=0;
	Ngps = 4;
	Tgps = 4*T;
	
	usecompass = true;

}

//---------------------------------------------------------
// Destructor

KalmanSensorFusion::~KalmanSensorFusion()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool KalmanSensorFusion::OnNewMail(MOOSMSG_LIST &NewMail)
{
    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++) 
	{
	    CMOOSMsg &msg = *p;
		string key = msg.GetKey();//Nome da mensagem
		//Medidas do Acelerometro
		if(key == "IMU_AX")
	  	{
	  		U_IMU[0] = msg.GetDouble();
	  	}
	    if(key == "IMU_AY")
	  	{
	  		U_IMU[1] = msg.GetDouble();
	  	}
	    if(key == "IMU_ATIME")
	  	{
	  		dt_accel = msg.GetDouble();
	  	}
		
		//Medida do Gyro
	    if(key == "IMU_WZ")
	  	{
	  		U_IMU[2] = msg.GetDouble();
	  	}
	    if(key == "IMU_WTIME")
	  	{
	  		dt_gyro = msg.GetDouble();
	  	}
		
		//Medida da Bussola
	    if(key == "IMU_HEADING")
	  	{
	  		compass = msg.GetDouble();
			//Convertendo para o referencial correto de psi
			psi_compass = (90 - compass)*pi/180;
			while(psi_compass<0 || psi_compass>2*pi)
			{
				if(psi_compass<0){psi_compass = psi_compass + 2*pi;}
				if(psi_compass>2*pi){psi_compass = psi_compass - 2*pi;}
			}
	  	}

		//Medidas do GPS
	    if(key == "GPS_X")
	  	{
	  		GPS[0] = msg.GetDouble();
	  	}
	    if(key == "GPS_Y")
	  	{
	  		GPS[1] = msg.GetDouble();
	  	}
		

		
	 }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool KalmanSensorFusion::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool KalmanSensorFusion::Iterate()
{
	m_iterations++;
	if(m_iterations!=0)
	{
		
		
		KalmanFilter();
		double vx = XK_INS[0];
		double vy = XK_INS[1];
		double px = XK_INS[2];
		double py = XK_INS[3];
		double psi = XK_INS[4];
		
		//Comparisson to ignore or not the psi estimated by the kalman filter
		double heading;
		if (usecompass)
		{
			heading = (90 - psi_compass*180/pi);
		}
		else
		{
			heading = (90 - psi*180/pi);
		}
		//Back to the correct frame
		while(heading<0 || heading>360)
		{
			if(heading<0){heading = heading + 360;}
			if(heading>360){heading = heading - 360;}
		}
		double speed = sqrt(vx*vx + vy*vy);
		//Notificando o MOOSDB
		for(int i=0;i<8;i++)
		{
			m_Comms.Notify("X_ERRO["+to_string(i)+"]", X_ERRO[i]);//m
			m_Comms.Notify("PK["+to_string(i)+"]", PK(i,i));//m
		}
		m_Comms.Notify("NAV_X", px);//m
	  	m_Comms.Notify("NAV_Y",py);//m
		m_Comms.Notify("NAV_HEADING",heading);//degree
		m_Comms.Notify("NAV_SPEED",speed);//m/s
		
	
	}	
	return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool KalmanSensorFusion::OnStartUp()
{
    cout<<"Startup"<<endl;
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
		  //A principio nao recebe nenhum parametro de startup
		  	if(param == "BETA")
			{
	  		  std::string valor = value;
	  		  KalmanBeta = std::stod(valor.c_str());
			}
		  	if(param == "P0")
			{
				std::string str = value;
				std::istringstream ss(str);
				string token;
				std::vector<string> vect;
				while(std::getline(ss, value, ','))
				{
			        vect.push_back(value);
				}
				//Passando o valor recuperado para a matriz P0
				for (int i =0; i<vect.size(); i++)
				{
					double valor = std::stod(vect[i].c_str());
					P0(i,i) = valor;
				}
				cout << P0 <<endl;
			}
		  	if(param == "GPS_VAR")
			{
  	  		  std::string valor = value;
  	  		  GPSvar = std::stod(valor.c_str());
			  
			}
		  	if(param == "COMPASS_VAR")
			{
  	  		  std::string valor = value;
  	  		  COMPASSvar = std::stod(valor.c_str());
			}
		  	if(param == "ACCEL_VAR")
			{
  	  		  std::string valor = value;
  	  		  ACCELvar = std::stod(valor.c_str());
			}
		  	if(param == "GYRO_VAR")
			{
  	  		  std::string valor = value;
  	  		  GYROvar = std::stod(valor.c_str());
			}
			
		  	if(param == "USE_COMPASS")
			{
  	  		  if(value == "YES" || value == "Yes" || value == "yes" ) usecompass = true;
			  else usecompass = false;
			}
		}
		
		//Updating the Variance matrix
		VarCOMP << COMPASSvar;//Compass variance
		VarGPS << GPSvar,GPSvar;//GPS variance	
		VarIMU << ACCELvar, ACCELvar, GYROvar;//Accel and gyro variance [Axb Ayb Wz]
		QK = VarIMU.asDiagonal();
		RK << VarGPS[0], 	0,	 	0,
				0,		VarGPS[1],	0,
				0,			0,		VarCOMP[0];
		cout<<"Rk = " << RK <<endl;
		cout<<"Qk = " << QK <<endl;
    }
  
    m_timewarp = GetMOOSTimeWarp();

    RegisterVariables();	
    return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void KalmanSensorFusion::RegisterVariables()
{
	//Register for variables
	//Acelerometro
	m_Comms.Register("IMU_AX", 0);
	m_Comms.Register("IMU_AY", 0);
	m_Comms.Register("IMU_AZ", 0);
	m_Comms.Register("IMU_ATIME", 0);
	//Giroscopio
	m_Comms.Register("IMU_WX", 0);
	m_Comms.Register("IMU_WY", 0);
	m_Comms.Register("IMU_WZ", 0);
	m_Comms.Register("IMU_WTIME", 0);
	//Compass
	m_Comms.Register("IMU_HEADING", 0);
	//GPS
	m_Comms.Register("GPS_X", 0);
	m_Comms.Register("GPS_Y", 0);
}


//---------------------------------------------------------
//Kalman FIlter Section
	
void KalmanSensorFusion::KalmanFilter()
{
	U_INS = U_IMU - BiasK;//[abx aby wz]
	//cout << "BiasAx: " << BiasK[0] <<endl;
	//cout << "Wz: " << U_INS[2] <<endl;
	//Integration of the model
	x[0]=XK_INS[0];
	x[1]=XK_INS[1];
	x[2]=XK_INS[2];
	x[3]=XK_INS[3];
	x[4]=XK_INS[4];
	double dt = T/10;
	for( double t=0.0 ; t<T ; t+= dt )
		stepper.do_step([this]( const state_type &x , state_type &dxdt , const double  t)
		{
			double abx = U_INS[0];
			double aby = U_INS[1];
			double wz = U_INS[2];
			double vx = x[0];
			double vy = x[1];
			double psi = x[4];
			//dxINS equation
			dxdt[0] = abx*cos(psi) - aby*sin(psi); 
			dxdt[1] = abx*sin(psi) + aby*cos(psi);
			dxdt[2] = vx; 
			dxdt[3] = vy;
			dxdt[4] = wz;
	 	}, x , t , dt );
 	// Saving the output of the integration in another variable
	XK1_INS[0] = x[0];
	XK1_INS[1] = x[1];
	XK1_INS[2] = x[2];
	XK1_INS[3] = x[3];
	XK1_INS[4] = x[4];

	BiasK1 = BiasK;

	//Kalman filter propagation
	// Dynamic errors matrix
	double x5ins = XK_INS[4];
	double alpha = cos(x5ins);
	double beta = sin(x5ins);
	//Discrete matrix obtained using the software Mathematica 
	MatrixXd Ad_ERROS;
	MatrixXd Ad_ERROS_T;//Transpose of Ad_ERROS
	Ad_ERROS.resize(8,8);
	Ad_ERROS_T.resize(8,8);
	Ad_ERROS<< 1, 0, 0, 0, 0,	T*alpha,		-T*beta,			0,
			   0, 1, 0, 0, 0,   T*beta,      	 T*alpha, 			0,
			   T, 0, 1, 0, 0,   T*T*alpha/2, 	-T*T*beta/2,		0,
			   0, T, 0, 1, 0,   T*T*beta/2,	 	 T*T*alpha/2,		0,
			   0, 0, 0, 0, 1, 		0,				0,				T,	
			   0, 0, 0, 0, 0, 		1,				0,				0,
			   0, 0, 0, 0, 0, 		0,				1,				0,
			   0, 0, 0, 0, 0, 		0,				0,				1;
	Ad_ERROS_T = Ad_ERROS.transpose();
	
	MatrixXd Bd_ERROS;
	MatrixXd Bd_ERROS_T;//Transpose
	Bd_ERROS.resize(8,3);
	Bd_ERROS_T.resize(3,8);
	Bd_ERROS << T*alpha, 		   -T*beta, 			0,
			    T*beta, 			T*alpha, 			0,
			    T*T*alpha/2,	   -T*T*beta/2,			0,
			   	T*T*beta/2,			T*T*alpha/2,		0,
		    		0,					0,				T,	
		  		  	0,					0,				0,
		   			0,					0,				0,
		    		0,					0,				0;
	Bd_ERROS_T = Bd_ERROS.transpose();


	PK1 = Ad_ERROS*PK*Ad_ERROS_T + Bd_ERROS*QK*T*Bd_ERROS_T + KalmanBeta*P0;

	
	ic = ic +1;
	if(ic==Ngps)
	{
		// Gk  = Pk*C_T*(C*Pk*C_T + Rk)^-1
		MatrixXd GK; GK.resize(8,3);
		MatrixXd inter;inter.resize(3,3);
		MatrixXd inter_inv;inter_inv.resize(3,3);
		inter = C_ERROS*PK1*C_ERROS_T + RK; 
		inter_inv = inter.inverse();//Good until 3x3 matrix
		GK = PK1*C_ERROS_T*inter_inv;
		
		MatrixXd pk1_inter;pk1_inter.resize(8,8);
		pk1_inter = (I8 - GK*C_ERROS)*PK1;
		PK1 = pk1_inter;
		
		Vector3d XINS_3_4_5; XINS_3_4_5 << XK_INS[2], XK_INS[3], XK_INS[4]; 
		Vector3d GPS_COMPASS; GPS_COMPASS << GPS[0], GPS[1], psi_compass;
		Y_ERRO =  XINS_3_4_5 - GPS_COMPASS;
		
		VectorXd ZeroBias;ZeroBias.resize(8);ZeroBias << 0, 0, 0, 0, 0, BiasK1[0], BiasK1[1], BiasK1[2];
		
		X_ERRO = ZeroBias + GK*Y_ERRO;
		
		
		VectorXd XerroINS; XerroINS.resize(5); XerroINS << X_ERRO[0],X_ERRO[1],X_ERRO[2],X_ERRO[3],X_ERRO[4];
		XK1_INS = XK1_INS - XerroINS;
		
		BiasK1[0] = X_ERRO[5];
		BiasK1[1] = X_ERRO[6];
		BiasK1[2] = X_ERRO[7];
		ic=0;
	}
	
	//Next kalman filter step
	PK = PK1;
	XK_INS = XK1_INS;
	BiasK = BiasK1;
}
	

