
#ifndef __TesteSerial__SerialPort__
#define __TesteSerial__SerialPort__

#include <iostream>
#include <boost/asio.hpp>
#include <cstring>

class SerialPort
{
public:
    //Methods
    SerialPort();
	~SerialPort();
	void SerialOpen(std::string port, unsigned int baud_rate);
	std::string SerialRead();
    void SerialWrite(std::string s);
	bool isOpen();
	void Close();
	
	
private:
	boost::asio::io_service *io;
	boost::asio::serial_port *serial;
		
    

    
};
#endif 