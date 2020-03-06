#include <syscalls.h>

using namespace common;
using namespace hardwarecom;

SyscallHandler::SyscallHandler(InterruptManager* interruptManager, uint8_t interruptNum)
: InterruptHandler(interruptNum + interruptManager->HardwareInterruptOffset(), interruptManager)
{

}
SyscallHandler::~SyscallHandler(){

}
void printf(char*);

uint32_t SyscallHandler::HandlerInterrupt(uint32_t esp){
    CPUState* cpu = (CPUState*) esp;
    switch (cpu->eax)
    {
        case 4:
            printf((char*)cpu->ebx);
            break;
        
        default:
            break;
    }
    return esp;
}