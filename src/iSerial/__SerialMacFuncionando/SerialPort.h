//
//  SerialPort.h
//  TesteSerial

//  Codigo adptado de https://developer.apple.com/library/mac/documentation/devicedrivers/conceptual/WorkingWSerial/WWSerial_SerialDevs/SerialDevices.html

// Update: v1.01 desta classe esta funcionando bem com o sinal do GPS e o terminal!!!!!!
//
//
//


#ifndef __TesteSerial__SerialPort__
#define __TesteSerial__SerialPort__

//////////////// CLASSE PORTA SERIAL //////////////////////////////
#include <iostream>
#include "Definitions.h"
#include <cstring>

class SerialPort
{
public:
    //Methods
    SerialPort();
    void SerialChangePort(std::string caminho);
    void SerialWrite(std::string sentence);
    int SerialBegin(speed_t BaudRate);
    void SerialClose();
    void SetOptions(int x);
    bool IsOpen(){if(Descriptor!=-1) {return true;} return false;}
    std::string SerialRead();
    speed_t whichBaud(int option);
    std::string SerialReadChar();

    
private:
    std::string dispositivo;
    int Descriptor;
    speed_t BR;
    speed_t BRin;
    speed_t BRout;
    struct termios options;
    //funcoes privativas
    int SerialBeginAux(speed_t BaudRate);

    
};
/////////////////////////////////////////////////////////////////////////////////////////








#endif /* defined(__TesteSerial__SerialPort__) */
