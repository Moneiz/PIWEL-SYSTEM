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
        static InterruptManager* ActiveInterruptManager;
        InterruptHandler* handlers[256];

        struct GateDescriptor{
            uint16_t handlerAddressLowBits;
            uint16_t gdt_codeSegmentSelector;
            uint8_t reserved;
            uint8_t access;
            uint16_t handlerAddressHighBits;
        } __attribute__((packed));

        static GateDescriptor InterruptDescriptorTable[256];

        struct interruptDescriptorTablePointer
        {
            uint16_t size;
            uint32_t base;
        } __attribute__((packed));
        
        uint16_t hardwareInterruptOffset;
        static void SetInterruptDescriptorTableEntry(
            uint8_t interruptNumber,
            uint16_t codeSegmentSelectorOffset,
            void (*handler)(),
            uint8_t DescriptorPrivilegeLevel,
            uint8_t DescriptorType
        ); 

        static void IgnoreInterruptRequest();

        static void HandlerInterruptRequest0x00();
        static void HandlerInterruptRequest0x01();
        static void HandlerInterruptRequest0x02();
        static void HandlerInterruptRequest0x03();
        static void HandlerInterruptRequest0x04();
        static void HandlerInterruptRequest0x05();
        static void HandlerInterruptRequest0x06();
        static void HandlerInterruptRequest0x07();
        static void HandlerInterruptRequest0x08();
        static void HandlerInterruptRequest0x09();
        static void HandlerInterruptRequest0x0A();
        static void HandlerInterruptRequest0x0B();
        static void HandlerInterruptRequest0x0C();
        static void HandlerInterruptRequest0x0D();
        static void HandlerInterruptRequest0x0E();
        static void HandlerInterruptRequest0x0F();

        static uint32_t handlerInterrupt(uint8_t interruptNumber,uint32_t esp);
        uint32_t doHandlerInterrupt(uint8_t interruptNumber,uint32_t esp);

        Port8BitSlow picMasterCommand;
        Port8BitSlow picMasterData;
        Port8BitSlow picSlaveCommand;
        Port8BitSlow picSlaveData;  

        
     

    public:
        InterruptManager(uint16_t HardwareInterruptOffset, GlobalDescriptorTable* gdt);
        ~InterruptManager();
        uint16_t HardwareInterruptOffset();
        void Activate();
        void Deactivate();

        
};

#endif