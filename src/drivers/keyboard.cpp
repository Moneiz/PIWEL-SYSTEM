#include <drivers/keyboard.h>

using namespace common;
using namespace hardwarecom;

void printf(char* str);
void printfHex(uint8_t);

namespace drivers{

   

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
        Port16Bit shutdown(0x4004);
        if(handler == 0){
            return esp;
        }

        if(key < 0x80){
            switch (key)
            {
                case 0x01 : 
                
            	shutdown.Write(0x3400);
                break;
                case 0x02: handler->OnKeyDown('1'); break;
                case 0x03: handler->OnKeyDown('2'); break;
                case 0x04: handler->OnKeyDown('3'); break;
                case 0x05: handler->OnKeyDown('4'); break;
                case 0x06: handler->OnKeyDown('5'); break;
                case 0x07: handler->OnKeyDown('6'); break;
                case 0x08: handler->OnKeyDown('7'); break;
                case 0x09: handler->OnKeyDown('8'); break;
                case 0x0A: handler->OnKeyDown('9'); break;
                case 0x0B: handler->OnKeyDown('0'); break;

                case 0x0E: handler->OnKeyDown(0x7F); break;

                case 0x10: handler->OnKeyDown('A'); break;
                case 0x12: handler->OnKeyDown('E'); break;
                case 0x13: handler->OnKeyDown('R'); break;
                case 0x14: handler->OnKeyDown('T'); break;
                case 0x16: handler->OnKeyDown('U'); break;
                case 0x17: handler->OnKeyDown('I'); break;
                case 0x19: handler->OnKeyDown('P'); break;
                case 0x20: handler->OnKeyDown('D'); break;
                case 0x27: handler->OnKeyDown('M'); break; 
                case 0x2C: handler->OnKeyDown('W'); break;

                case 0x1C: handler->OnKeyDown('\n'); break;

                case 0x39: handler->OnKeyDown(' '); break;

                case 0x4F: handler->OnKeyDown('1'); break;
                case 0x50: handler->OnKeyDown('2'); break;
                case 0x51: handler->OnKeyDown('3'); break;
                case 0x4B: handler->OnKeyDown('4'); break;
                case 0x4C: handler->OnKeyDown('5'); break;
                case 0x4D: handler->OnKeyDown('6'); break;
                case 0x47: handler->OnKeyDown('7'); break;
                case 0x48: handler->OnKeyDown('8'); break;
                case 0x49: handler->OnKeyDown('9'); break;
                case 0x52: handler->OnKeyDown('0'); break;


                default:
                    //handler->OnKeyDown('?');
                    printfHex(key);
                    break;
            }
        }   

        
        return esp;
    }
}