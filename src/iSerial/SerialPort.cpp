#include "SerialPort.h"

/**
     * Constructor.
     * \param port device name, example "/dev/ttyUSB0" or "COM4"
     * \param baud_rate communication speed, example 9600 or 115200
     * \throws boost::system::system_error if cannot open the
     * serial device
*/

SerialPort::SerialPort()
{
  
}
SerialPort::~SerialPort()
{
	delete io;
	delete serial;
}

void SerialPort::SerialOpen(std::string port, unsigned int baud_rate)
{
	io = new boost::asio::io_service();
	serial = new boost::asio::serial_port(*io, port);
		
	serial->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}
   
/**
     * Write a string to the serial device.
     * \param s string to write
     * \throws boost::system::system_error on failure
 */

void SerialPort::SerialWrite(std::string s)
{
    boost::asio::write(*serial,boost::asio::buffer(s.c_str(),s.size()));
}

 /**
     * Blocks until a line is received from the serial device.
     * Eventual '\n' or '\r\n' characters at the end of the string are removed.
     * \return a string containing the received line
     * \throws boost::system::system_error on failure
     */
std::string SerialPort::SerialRead()
{
    //Reading data char by char, code is optimized for simplicity, not speed
    using namespace boost;
    char c;
    std::string result;
    for(;;)
    {
        asio::read(*serial,asio::buffer(&c,1));
        switch(c)
        {
            case '\r':
                break;
            case '\n':
                return result;
            default:
                result+=c;
        }
    }
}

bool SerialPort::isOpen()
{
	return serial->is_open();
}

void SerialPort::Close()
{
	serial->close();
}
