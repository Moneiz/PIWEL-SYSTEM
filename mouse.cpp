#include "mouse.h"

void printf(char* str);

MouseDriver::MouseDriver(InterruptManager* manager) : 
InterruptHandler(0x2C,manager),
dataport(0x60),
commandport(0x64)
{

    offset = 0;
    buttons = 0;

    uint16_t* VideoMemory = (uint16_t*) 0xB8000;

     VideoMemory[80*12+40] = ((VideoMemory[80*12+40] & 0xF000) >> 4) 
                | ((VideoMemory[80*12+40] & 0x0F00) << 4)
                | ((VideoMemory[80*12+40] & 0x00FF));

    commandport.Write(0xA8); // enable int
    commandport.Write(0x20); //get current state
    uint8_t status = (dataport.Read() | 2);
    commandport.Write(0x60); //set state
    dataport.Write(status);
    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read();
}
MouseDriver::~MouseDriver(){
    
}
uint32_t MouseDriver::HandlerInterrupt(uint32_t esp){

   
    uint8_t status = commandport.Read();
    if(!(status & 0x20)){
        return esp;
    }
    static int8_t x=40,y=12;

    buffer[offset] = dataport.Read();
    offset = (offset+1)%3;

    static uint16_t* VideoMemory = (uint16_t*)0xB8000;
    
    if(offset == 0){


        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) 
                | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                | ((VideoMemory[80*y+x] & 0x00FF));

        x+=buffer[1];
        y-=buffer[2];

        if(x < 0) x = 0;
        if(x >= 80) x = 79;
        if(y < 0) y = 0;
        if(y >= 25) y = 24;

        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) 
                | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                | ((VideoMemory[80*y+x] & 0x00FF));
    }
    for(uint8_t i = 0; i < 3;i++){
        if((buffer[0] & (0x01 << i)) != (buttons & (0x01<<i))){
            if(i==0){
                printf("CLIQUE SOURIS 1\n");
                //VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0x0F00)>>1);
            }
        }
    }

    return esp;
}
