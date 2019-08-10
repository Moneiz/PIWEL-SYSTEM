#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager;

class InterruptHandler{
    protected:
        uint8_t interruptNumber;
        InterruptManager* interruptManager;

        InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager);
        ~InterruptHandler();
    public:
        virtual uint32_t HandlerInterrupt(uint32_t esp);
};

class InterruptManager{
    friend class InterruptHandler;
    protected:

        struct GateDescriptor{
            uint16_t handlerAddressLowBits;
            uint16_t gdt_codeSegmentSelector;
            uint8_t reserved;
            uint8_t access;
            uint16_t handlerAddressHighBits;
        } __attribute__((packed));

        static GateDescriptor InterruptDescriptorTable[256];
        InterruptHandler* handlers[256];

        struct interruptDescriptorTablePointer
        {
            uint16_t size;
            uint32_t base;
        } __attribute__((packed));
        

        static void SetInterruptDescriptorTableEntry(
            uint8_t interruptNumber,
            uint16_t codeSegmentSelectorOffset,
            void (*handler)(),
            uint8_t DescriptorPrivilegeLevel,
            uint8_t DescriptorType
        );

        Port8BitSlow picMasterCommand;
        Port8BitSlow picMasterData;
        Port8BitSlow picSlaveCommand;
        Port8BitSlow picSlaveData;        

    public:
    
        static InterruptManager* ActiveInterruptManager;

        InterruptManager(GlobalDescriptorTable* gdt);
        ~InterruptManager();

        static uint32_t handlerInterrupt(uint8_t interruptNumber,uint32_t esp);
        uint32_t doHandlerInterrupt(uint8_t interruptNumber,uint32_t esp);

        void Activate();
        void Deactivate();

        static void IgnoreInterruptRequest();
        static void HandlerInterruptRequest0x00();
        static void HandlerInterruptRequest0x01();
        static void HandlerInterruptRequest0x0C();
};

#endif