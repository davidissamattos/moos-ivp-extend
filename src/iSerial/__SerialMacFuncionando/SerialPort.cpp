//
//  SerialPort.cpp
//  TesteSerial
//  Codigo adptado de https://developer.apple.com/library/mac/documentation/devicedrivers/conceptual/WorkingWSerial/WWSerial_SerialDevs/SerialDevices.html

// Update: v1.0 desta classe esta funcionando bem com o sinal do GPS e o terminal!!!!!!
//
//
//


#include "SerialPort.h"
#include <cstring>


/////////////////// Constructor /////////////////////////////////

SerialPort::SerialPort()
{

}
////////////////////// Change Port //////////////////////////////

void SerialPort::SerialChangePort(std::string caminho)
{
dispositivo = caminho;
}

/////////////////// SerialBegin ///////////////////////////////////
int SerialPort::SerialBeginAux(speed_t BaudRate)
{
    const char *deviceFilePath;
    deviceFilePath = dispositivo.c_str();
    
    int         fileDescriptor = -1;
    int         handshake;
    fileDescriptor = open(deviceFilePath, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fileDescriptor == -1)
    {
        printf("Error opening serial port %s - %s(%d).\n",
               deviceFilePath, strerror(errno), errno);
        goto error;
    }
    
                    // Note that open() follows POSIX semantics: multiple open() calls to
                    // the same file will succeed unless the TIOCEXCL ioctl is issued.
                    // This will prevent additional opens except by root-owned processes.
                    // See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
    
                    if (ioctl(fileDescriptor, TIOCEXCL) == kMyErrReturn)
                        {
                            printf("Error setting TIOCEXCL on %s - %s(%d).\n",
                            deviceFilePath, strerror(errno), errno);
                            goto error;
                        }
    
                    // Now that the device is open, clear the O_NONBLOCK flag so
                    // subsequent I/O will block.
                    // See fcntl(2) ("man 2 fcntl") for details.
    
                    if (fcntl(fileDescriptor, F_SETFL, 0) == kMyErrReturn)
                    {
                        printf("Error clearing O_NONBLOCK %s - %s(%d).\n",deviceFilePath,strerror(errno), errno);
                        goto error;
                    }
    
                    // Get the current options and save them so we can restore the
                    // default settings later.
                    if (tcgetattr(fileDescriptor, &gOriginalTTYAttrs) == kMyErrReturn)
                    {
                        printf("Error getting tty attributes %s - %s(%d).\n",deviceFilePath, strerror(errno), errno);
                        goto error;
                    }
    
                    // The serial port attributes such as timeouts and baud rate are set by
                    // modifying the termios structure and then calling tcsetattr to
                    // cause the changes to take effect. Note that the
                    // changes will not take effect without the tcsetattr() call.
                    // See tcsetattr(4) ("man 4 tcsetattr") for details.
    
                    options = gOriginalTTYAttrs;
    
                    // Print the current input and output baud rates.
                    // See tcsetattr(4) ("man 4 tcsetattr") for details.
                    //printf("Current input baud rate is %d\n", (int) cfgetispeed(&options));
                    //printf("Current output baud rate is %d\n", (int) cfgetospeed(&options));
                    // Set raw input (non-canonical) mode, with reads blocking until either
                    // a single character has been received or a one second timeout expires.
                    // See tcsetattr(4) ("man 4 tcsetattr") and termios(4) ("man 4 termios")
                    // for details.
    
                    cfmakeraw(&options);
                    options.c_cc[VMIN] = 1;
                    options.c_cc[VTIME] = 10;
    
                    // The baud rate, word length, and handshake options can be set as follows:
    
    //Setando as configuracoes de BAUDRATE padrao para
        cfsetspeed(&options, BaudRate);
		cfsetospeed(&options, BaudRate);
		cfsetspeed(&options, BaudRate);
	
        tcsetattr(Descriptor, TCSAFLUSH, &options);//Set Flush
        tcsetattr(Descriptor, TCSANOW, &options);//Set options NOW


    //conferindo o baudrate
        printf("Current input baud rate is %d\n", (int) cfgetispeed(&options));
        printf("Current output baud rate is %d\n", (int) cfgetospeed(&options));
   //Recupera os baud rates setados
    	BRin=cfgetispeed(&options);
    	BRout=cfgetospeed(&options);

                    // Cause the new options to take effect immediately.
                    if (tcsetattr(fileDescriptor, TCSANOW, &options) == kMyErrReturn)
                    {
                        printf("Error setting tty attributes %s - %s(%d).\n", deviceFilePath, strerror(errno), errno);
                        goto error;
                    }
    
                        // To set the modem handshake lines, use the following ioctls.
                        // See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
    
                    handshake = TIOCM_DTR | TIOCM_RTS | TIOCM_CTS | TIOCM_DSR;
                    // Set the modem lines depending on the bits set in handshake.
                    if (ioctl(fileDescriptor, TIOCMSET, &handshake) == kMyErrReturn)
                    {
                        printf("Error setting handshake lines %s - %s(%d).\n", deviceFilePath, strerror(errno), errno);
                    }
    
                    // To read the state of the modem lines, use the following ioctl.
                    // See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
    
                    if (ioctl(fileDescriptor, TIOCMGET, &handshake) == kMyErrReturn)
                        // Store the state of the modem lines in handshake.
                    {
                        printf("Error getting handshake lines %s - %s(%d).\n",deviceFilePath, strerror(errno), errno);
                    }
    
    //printf("Handshake lines currently set to %d\n", handshake);
    
                    // Success:
                    return fileDescriptor;
    
                    // Failure:
                    error:
                    if (fileDescriptor != kMyErrReturn)
                    {
                        close(fileDescriptor);
                    }
                    return -1;
}

int SerialPort::SerialBegin(speed_t BaudRate)
{
    BR = BaudRate;
    Descriptor = SerialBeginAux(BR);
    
    tcflush(Descriptor, TCIOFLUSH);//remove o que esta armazenado no buffer ao conectar
    
    return Descriptor;
}


///////////////////////////// SerialClose /////////////////////////////

void SerialPort::SerialClose()
{
    int fileDescriptor =Descriptor;
    // Block until all written output has been sent from the device.
    // Note that this call is simply passed on to the serial device driver.
    // See tcsendbreak(3) ("man 3 tcsendbreak") for details.
    if (tcdrain(fileDescriptor) == kMyErrReturn)
    {
        printf("Error waiting for drain - %s(%d).\n",
               strerror(errno), errno);
    }
    
    // It is good practice to reset a serial port back to the state in
    // which you found it. This is why we saved the original termios struct
    // The constant TCSANOW (defined in termios.h) indicates that
    // the change should take effect immediately.
    if (tcsetattr(fileDescriptor, TCSANOW, &gOriginalTTYAttrs) ==
        kMyErrReturn)
    {
        printf("Error resetting tty attributes - %s(%d).\n",
               strerror(errno), errno);
    }
    
    close(fileDescriptor);
}


/////////////////////////////// SerialWrite() /////////////////////////////////
void SerialPort::SerialWrite(std::string sentence)
{
    const char *buffer;
    buffer = sentence.c_str();
    
    speed_t len = sentence.length();
    speed_t n;
 
    n=write(Descriptor, buffer,len);
}

/////////////////////////// SerialReadChar()  ////////////////////////////////
//NAO MEXER -> Funcionando
std::string SerialPort::SerialReadChar()
{
    speed_t n;
    char* buffer=new char[1];
    buffer[0] = '\0';
    //ler so um caracter por vez
    n=read(Descriptor, buffer, 1);
    std::string valor;
    valor = buffer;
    delete buffer;
    return valor;
}

//////////////////////////////// SerialRead() /////////////////////////////
//Le uma palavra inteira
//TO DO: arrumar para ler frase com inteiras
std::string SerialPort::SerialRead()
{
    std::string aux;
    aux = SerialReadChar();
    std::string final;
    final="\0";
//Essa condicao do \r funciona bem para o arduino que sempre envia um \r e um \n no final (se estiver usando o Serial.println
    while(aux.compare("\r") && aux.compare("\0") && aux.compare("\n"))
    {    
        final = final + aux;
        aux = SerialReadChar();
       // if (!aux.compare(" "))
        //{
          //  final = final + " ";
            //aux = SerialReadChar();
        //}
    }
    return final;
}




//////////////////////////////// whichBaud() /////////////////////////////////////
// Opcao 1: para o BR de entrada
// Opcao 2: para o BR de saida
// Opcao 3: para o BR que eu escolhi para setar no SerialBegin

speed_t SerialPort::whichBaud(int option)
{
    switch (option)
    {
        case 1:
            return BRin;
            break;
        case 2:
            return BRout;
            break;
        case 3:
            return BR;
            break;
    }
    return -1;
}

//////////////////////////////// SetOptionsd() /////////////////////////////////////
// Opcao 1: 
// Opcao 2: 
// Opcao 3:

void SerialPort::SetOptions(int option)
{
	switch (option)
    	{
        	case 1://Usado para o arduino
        		options.c_cflag |= CS8;
       			options.c_cflag |= CLOCAL;
        		options.c_cflag |= CREAD;
                options.c_cflag &= ~PARENB;
                options.c_iflag = 0;
                options.c_oflag = 0;
                options.c_lflag = 0;
                tcsetattr(Descriptor, TCSAFLUSH, &options);
                tcsetattr(Descriptor, TCSANOW, &options);//Set options NOW

        	    break;
        	case 2://Usado para o GPS
                options.c_cflag |= CS8;
       			options.c_cflag |= CLOCAL;
        		options.c_cflag |= CREAD;
                options.c_cflag |= PARENB;
                options.c_iflag = 0;
                options.c_oflag = 0;
                options.c_lflag = 0;
                tcsetattr(Descriptor, TCSAFLUSH, &options);//Set Flush
                tcsetattr(Descriptor, TCSANOW, &options);//Set options NOW
	
        	    break;
        	case 3:
        	    
        	    break;
    	}
 /*
    //Setando as configuracoes de BAUDRATE
    	cfsetspeed(&options, BaudRate);   // Set baud Rate
    	BRin=cfgetispeed(&options);//Recupera os baud rates setados
    	BRout=cfgetospeed(&options);
    
    //Setando as configuracoes de controle
        options.c_cflag |= CS8;
        options.c_cflag |= CLOCAL;
        options.c_cflag |= CREAD;
    
        //options.c_cflag |= PARENB;//Habilita o bit de paridade Para o GPS
        //opcoes para o arduino
        options.c_cflag &= ~PARENB;
    
    //configuracoes de input
        options.c_iflag = 0;
        //options.c_cflag |= IGNCR;//ignore o carriage return
    
    
    //configuracoes de output
        options.c_oflag = 0;
        //options.c_oflag |= OPOST; //Sem pos processamento da saida
        //options.c_oflag |= CR3;// aguarda 150ms apos enviar uma saida
    
    //configuracoes local
        options.c_lflag = 0;
    
    //Habilita o flush para input e output (Nao sei para que que serve)
        tcsetattr(Descriptor, TCSAFLUSH, &options);
   //Set options NOW
	tcsetattr(Descriptor, TCSANOW, &options);

//olhar
if(BaudRate==4800)
	{
 		cfsetspeed(&options, B4800);
		cfsetospeed(&options, B4800);
		cfsetispeed(&options, B4800);
	}
	else if(BaudRate==9600)
	{
 		cfsetspeed(&options, B9600);
	}
	else if(BaudRate==2400)
	{
 		cfsetspeed(&options, B2400);
	}
	else if(BaudRate==38400)
	{
 		cfsetspeed(&options, B38400);
	}	
	else if(BaudRate==115200)
	{
	 	cfsetspeed(&options, B115200);
	}
    


*/
}
