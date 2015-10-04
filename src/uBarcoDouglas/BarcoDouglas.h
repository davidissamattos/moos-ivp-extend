/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN: ITA                                             */
/*    FILE: BarcoDouglas.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef BarcoDouglas_HEADER
#define BarcoDouglas_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "Model.h"
#include <ctime>

class BarcoDouglas : public CMOOSApp
{
public:
	BarcoDouglas();
	virtual ~BarcoDouglas();

	bool OnNewMail(MOOSMSG_LIST &NewMail);
	bool Iterate();
	bool OnConnectToServer();
	bool OnStartUp();
	void RegisterVariables();
	
	time_t tempo_atual;
	time_t tempo_ant;

protected:
    std::string v_MotorM1_name;//Guarda o nome da variavel MOOS responsavel por indicar o motor M1. O nome e definido no arquivo MOOS
	std::string v_MotorM2_name;//Guarda o nome da variavel MOOS responsavel por indicar o motor M2. O nome e definido no arquivo MOOS
    
	std::string m_myx_name;
	std::string m_myy_name;
	std::string m_myspeed_name;
	std::string m_myheading_name;
	std::string m_myturn_name;
	
	double myheading;
	double myd;
	double myx;
	double myy;
	double mylat;
	double mylon;
	double myspeed;
	double myw;
	double MotorM1;//Variavel que guarda o valor obtido para M1
	double MotorM2;//Variavel que guarda o valor obtido para M2
	

private:
	Model myModel;
    unsigned int m_iterations;
    double m_timewarp;
	void updateModelVars();
};

#endif 
