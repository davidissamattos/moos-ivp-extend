/************************************************************/
/*    NAME: David Issa Mattos                                              */
/*    ORGN:ITA                                           */
/*    FILE: ControleBarco.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef ControleBarco_HEADER
#define ControleBarco_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <cstring>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <cwctype>
#include <stdexcept>
#define pi 3.14159


void toUpper(std::basic_string<char>& s);
void toUpper(std::basic_string<wchar_t>& s);
void toLower(std::basic_string<char>& s);
void toLower(std::basic_string<wchar_t>& s);


class ControleBarco : public CMOOSApp
{
	public:
		ControleBarco();
		virtual ~ControleBarco();
		bool OnNewMail(MOOSMSG_LIST &NewMail);
		bool Iterate();
		bool OnConnectToServer();
		bool OnStartUp();
		void RegisterVariables();

	protected:
		

	private: // State variables
		double calcd(double x, double y);
		void Tabela();
		std::string m_myx_name;
		std::string m_myy_name;
		std::string m_myspeed_name;
		std::string m_myheading_name;
		std::string m_desired_heading_name;
		std::string m_desired_speed_name;
		
		std::string disable_controlebarco;
		
		unsigned int m_iterations;
		double m_timewarp;
		
		double myheading;
		double myd;
		double myx;
		double myy;
		double myspeed;
		double d;
		
		double desired_heading;
		double desired_d;
		double desired_x;
		double desired_y;
		double desired_speed;
		double desired_vx;
		double desired_vy;
		
		double delta_d;
		double delta_heading;
		
		double M1;//variaveis da tabela de acionamento
		double M2;
		int flag_proa;//Analogo ao script em Matlab do douglas
		
		time_t tempo_ant;
		time_t tempo_atual;
		double deltat;
};

#endif 
