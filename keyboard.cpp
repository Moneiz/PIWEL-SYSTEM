#include "keyboard.h"

void printf(char* str);

KeyboardDriver::KeyboardDriver(InterruptManager* manager) : 
InterruptHandler(0x21,manager),
dataport(0x60),
commandport(0x64)
{
    while(commandport.Read() & 0x1){
        dataport.Read();
    
    }
    commandport.Write(0xAE); // enable int
    commandport.Write(0x20); //get current state
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); //set state
    dataport.Write(status);
    dataport.Write(0xF4);
}
KeyboardDriver::~KeyboardDriver(){
    
}
uint32_t KeyboardDriver::HandlerInterrupt(uint32_t esp){

    uint8_t key = dataport.Read();

    if(key < 0x80){
        switch (key)
        {
        case 0xFA: break;
        case 0x45: case 0xC5:break;

        default:
            char* warn = "INTERRUPTION DU CLAVIER    \n";
            char* hex = "0123456789ABCDEF";
            warn[24] = hex[(key >> 4) & 0x0F] ;
            warn[25] = hex[(key) & 0x0F] ;
            printf(warn);
            break;
        }
    }   

    
    return esp;
}
