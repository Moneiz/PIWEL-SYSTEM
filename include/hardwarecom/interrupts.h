#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <common/types.h>
#include <hardwarecom/port.h>
#include <gdt.h>
#include <scheduling.h>

namespace hardwarecom{

    class InterruptManager;

    class InterruptHandler{
        protected:
            common::uint8_t interruptNumber;
            InterruptManager* interruptManager;

            InterruptHandler(common::uint8_t interruptNumber, InterruptManager* interruptManager);
            ~InterruptHandler();
        public:
            virtual common::uint32_t HandlerInterrupt(common::uint32_t esp);
    };

    class InterruptManager{
        friend class InterruptHandler;
        protected:
            static InterruptManager* ActiveInterruptManager;
            InterruptHandler* handlers[256];
            TaskManager *taskManager;
            struct GateDescriptor{
                common::uint16_t handlerAddressLowBits;
                common::uint16_t gdt_codeSegmentSelector;
                common::uint8_t reserved;
                common::uint8_t access;
                common::uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor InterruptDescriptorTable[256];

            struct interruptDescriptorTablePointer
            {
                common::uint16_t size;
                common::uint32_t base;
            } __attribute__((packed));
            
            common::uint16_t hardwareInterruptOffset;
            static void SetInterruptDescriptorTableEntry(
                common::uint8_t interruptNumber,
                common::uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                common::uint8_t DescriptorPrivilegeLevel,
                common::uint8_t DescriptorType
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

            static common::uint32_t handlerInterrupt(common::uint8_t interruptNumber,common::uint32_t esp);
            common::uint32_t doHandlerInterrupt(common::uint8_t interruptNumber,common::uint32_t esp);

            Port8BitSlow picMasterCommand;
            Port8BitSlow picMasterData;
            Port8BitSlow picSlaveCommand;
            Port8BitSlow picSlaveData;  

            
        

        public:
            InterruptManager(common::uint16_t HardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManager* taskManager);
            ~InterruptManager();
            common::uint16_t HardwareInterruptOffset();
            void Activate();
            void Deactivate();

            
    };

}
#endif