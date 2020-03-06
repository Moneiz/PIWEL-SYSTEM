#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#include <common/types.h>
#include <hardwarecom/interrupts.h>
#include <scheduling.h>

class SyscallHandler : public hardwarecom::InterruptHandler{
    public:
    SyscallHandler(hardwarecom::InterruptManager* interruptManager, common::uint8_t interruptNum);
    ~SyscallHandler();
    virtual common::uint32_t HandlerInterrupt(common::uint32_t esp);
};

#endif