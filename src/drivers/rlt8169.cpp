#include <drivers/rlt8169.h>

using namespace common;
using namespace drivers;
using namespace hardwarecom;

void printf(char*);
void printfHex(uint8_t);

rtl8169::rtl8169(PCIDeviceDescriptor *dev, InterruptManager* interrupts) :
Driver(),
InterruptHandler(dev->interrupt + (interrupts->HardwareInterruptOffset()),interrupts ),
MACAddress0Port(dev->portBase ),
MACAddress1Port(dev->portBase + 0x1),
MACAddress2Port(dev->portBase + 0x2),
MACAddress3Port(dev->portBase + 0x3),
MACAddress4Port(dev->portBase + 0x4),
MACAddress5Port(dev->portBase + 0x5)
{
    uint64_t MAC0 = MACAddress0Port.Read() & 0xFF;
    uint64_t MAC1 = MACAddress1Port.Read() & 0xFF;
    uint64_t MAC2 = MACAddress2Port.Read() & 0xFF;
    uint64_t MAC3 = MACAddress3Port.Read() & 0xFF;
    uint64_t MAC4 = MACAddress4Port.Read() & 0xFF;
    uint64_t MAC5 = MACAddress5Port.Read() & 0xFF;

    uint64_t MAC = MAC5 << 40
                | MAC4 << 32
                | MAC3 << 24
                | MAC2 << 16
                | MAC1 << 8
                | MAC0;

    // show mac addr
    printf("MAC :");
    printfHex(MAC0 & 0xFF);
    printfHex(MAC1 & 0xFF);
    printfHex(MAC2 & 0xFF);
    printfHex(MAC3 & 0xFF);
    printfHex(MAC4 & 0xFF);
    printfHex(MAC5 & 0xFF);
    printf("/n");

    sendBufferDescr = (BufferDescriptor*)((((uint32_t)&sendBufferDescrMemory[0]) + 15) & ~((uint32_t)0xF));
    
    recvBufferDescr = (BufferDescriptor*)((((uint32_t)&recvBufferDescrMemory[0]) + 15) & ~((uint32_t)0xF));
    
}

rtl8169::~rtl8169(){

}
void rtl8169::Activate(){
}