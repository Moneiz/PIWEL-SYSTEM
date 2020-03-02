#include <hardwarecom/interrupts.h>

using namespace common;

void printf(char* str);
void printfHex(uint8_t);

namespace hardwarecom{

    InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager){
        this->interruptManager = interruptManager;
        this->interruptNumber = interruptNumber;
        interruptManager->handlers[interruptNumber] = this;
    }
    InterruptHandler::~InterruptHandler(){
        if(interruptManager->handlers[interruptNumber] == this){
            interruptManager->handlers[interruptNumber] = 0;
        }
    }
    uint32_t InterruptHandler::HandlerInterrupt(uint32_t esp){
        return esp;   
    }

    InterruptManager::GateDescriptor InterruptManager::InterruptDescriptorTable[256];
    InterruptManager* InterruptManager::ActiveInterruptManager = 0;

    void InterruptManager::SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t codeSegmentSelectorOffset,
        void (*handler)(),
        uint8_t DescriptorPrivilegeLevel,
        uint8_t DescriptorType
    ){
        const uint8_t IDT_DESC_PRESENT = 0x80;

        InterruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
        InterruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler)>>16) & 0xFFFF;
        InterruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector= codeSegmentSelectorOffset;
        InterruptDescriptorTable[interruptNumber].reserved = 0;
        InterruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3)<<5);
    }
    InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManager* taskManager) :
    picMasterCommand(0x20),picMasterData(0x21),
    picSlaveCommand(0xA0), picSlaveData(0xA1)
    {
        this->hardwareInterruptOffset = hardwareInterruptOffset;
        this->taskManager = taskManager;
        uint32_t CodeSegment = gdt->CodeSegmentSelector();
        const uint8_t IDT_INTERRUPT_GATE = 0xE;
        for(uint8_t i = 255; i > 0;--i){
            handlers[i] = 0;
            SetInterruptDescriptorTableEntry(i,CodeSegment,&IgnoreInterruptRequest,0,IDT_INTERRUPT_GATE);
        }
        SetInterruptDescriptorTableEntry(0,CodeSegment, &IgnoreInterruptRequest,0, IDT_INTERRUPT_GATE);
        handlers[0] = 0;


        SetInterruptDescriptorTableEntry(0x00, CodeSegment, &HandleExceptionRequest0x00,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x01, CodeSegment, &HandleExceptionRequest0x01,0, IDT_INTERRUPT_GATE);        
        SetInterruptDescriptorTableEntry(0x02, CodeSegment, &HandleExceptionRequest0x02,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x03, CodeSegment, &HandleExceptionRequest0x03,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x04, CodeSegment, &HandleExceptionRequest0x04,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x05, CodeSegment, &HandleExceptionRequest0x05,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x06, CodeSegment, &HandleExceptionRequest0x06,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x07, CodeSegment, &HandleExceptionRequest0x07,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x08, CodeSegment, &HandleExceptionRequest0x08,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x09, CodeSegment, &HandleExceptionRequest0x09,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandleExceptionRequest0x0A,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandleExceptionRequest0x0B,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandleExceptionRequest0x0C,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandleExceptionRequest0x0D,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandleExceptionRequest0x0E,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandleExceptionRequest0x0F,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x10, CodeSegment, &HandleExceptionRequest0x10,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x11, CodeSegment, &HandleExceptionRequest0x11,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x12, CodeSegment, &HandleExceptionRequest0x12,0, IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(0x13, CodeSegment, &HandleExceptionRequest0x13,0, IDT_INTERRUPT_GATE);



        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+ 0x00,CodeSegment,&HandlerInterruptRequest0x00,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x01,CodeSegment,&HandlerInterruptRequest0x01,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x02,CodeSegment,&HandlerInterruptRequest0x02,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+ 0x03,CodeSegment,&HandlerInterruptRequest0x03,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x04,CodeSegment,&HandlerInterruptRequest0x04,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x05,CodeSegment,&HandlerInterruptRequest0x05,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+ 0x06,CodeSegment,&HandlerInterruptRequest0x06,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x07,CodeSegment,&HandlerInterruptRequest0x07,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x08,CodeSegment,&HandlerInterruptRequest0x08,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+ 0x09,CodeSegment,&HandlerInterruptRequest0x09,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x0A,CodeSegment,&HandlerInterruptRequest0x0A,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x0B,CodeSegment,&HandlerInterruptRequest0x0B,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+ 0x0C,CodeSegment,&HandlerInterruptRequest0x0C,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x0D,CodeSegment,&HandlerInterruptRequest0x0D,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+0x0E,CodeSegment,&HandlerInterruptRequest0x0E,0,IDT_INTERRUPT_GATE);
        SetInterruptDescriptorTableEntry(hardwareInterruptOffset+ 0x0F,CodeSegment,&HandlerInterruptRequest0x0F,0,IDT_INTERRUPT_GATE);

        picMasterCommand.Write(0X11);
        picSlaveCommand.Write(0X11);

        picMasterData.Write(hardwareInterruptOffset);
        picSlaveData.Write(hardwareInterruptOffset+8);

        picMasterData.Write(0x04);
        picSlaveData.Write(0x02);

        picMasterData.Write(0x01);
        picSlaveData.Write(0x01);

        picMasterData.Write(0x00);
        picSlaveData.Write(0x00);

        interruptDescriptorTablePointer idt;
        idt.size = 256 * sizeof(GateDescriptor) -1;
        idt.base = (uint32_t) InterruptDescriptorTable;

        asm volatile("lidt %0" :  : "m" (idt));
    }
    InterruptManager::~InterruptManager(){
        Deactivate();
    }

    uint16_t InterruptManager::HardwareInterruptOffset(){
        return hardwareInterruptOffset;
    }

    void InterruptManager::Activate(){

        if(ActiveInterruptManager != 0){
            ActiveInterruptManager->Deactivate();
        }

        ActiveInterruptManager = this;
        asm("sti");
    }


    void InterruptManager::Deactivate(){

        if(ActiveInterruptManager == this){
            ActiveInterruptManager = 0;
            asm("cli");
        }
    }

    uint32_t InterruptManager::handlerInterrupt(uint8_t interruptNumber,uint32_t esp){
        if(ActiveInterruptManager != 0){
            return ActiveInterruptManager->doHandlerInterrupt(interruptNumber,esp);
        }

        return esp;
    }
    uint32_t InterruptManager::doHandlerInterrupt(uint8_t interruptNumber,uint32_t esp){
        
        if(handlers[interruptNumber] != 0){
            printf("INTERUPTION GERE 0x");

            printfHex(interruptNumber);
            esp = handlers[interruptNumber]->HandlerInterrupt(esp);
        }

        else if(interruptNumber != hardwareInterruptOffset){
            printf("INTERUPTION NON-GERE 0x");
            printfHex(interruptNumber);
        }
        if(interruptNumber == hardwareInterruptOffset){
            esp = (uint32_t) taskManager->Schedule((CPUState*)esp);
        }

        if(hardwareInterruptOffset <= interruptNumber && interruptNumber <hardwareInterruptOffset+16){
            picMasterCommand.Write(0x20);
            if(hardwareInterruptOffset +8 <= interruptNumber){
                picSlaveCommand.Write(0x20);
            }
        }

        return esp;
    }
}