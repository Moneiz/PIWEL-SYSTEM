#include "keyboard.h"

void printf(char* str);
void printfHex(uint8_t);

KeyboardEventHandler::KeyboardEventHandler(){

}
void KeyboardEventHandler::OnKeyDown(char){

}
void KeyboardEventHandler::OnKeyUp(char){

}

KeyboardDriver::KeyboardDriver(InterruptManager* manager, KeyboardEventHandler* handler) : 
InterruptHandler(0x21,manager),
dataport(0x60),
commandport(0x64)
{
    this->handler = handler;
}
KeyboardDriver::~KeyboardDriver(){
    
}

void KeyboardDriver::Activate(){
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

uint32_t KeyboardDriver::HandlerInterrupt(uint32_t esp){

    uint8_t key = dataport.Read();

    if(handler == 0){
        return esp;
    }

    if(key < 0x80){
        switch (key)
        {
        case 0x39: asm("hlt" : : );
        case 0x45: case 0xC5:break;

        default:
            printfHex(key);
            break;
        }
    }   

    
    return esp;
}
