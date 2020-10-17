#ifndef __RTL_8169_H
#define __RTL_8169_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecom/pci.h>
#include <hardwarecom/interrupts.h>
#include <hardwarecom/port.h>

namespace drivers{

    class rtl8169;
    
    class rtl8169 : public Driver, public hardwarecom::InterruptHandler{
        struct BufferDescriptor{
            common::uint32_t address;
            common::uint32_t flags;
            common::uint32_t flags2;
            common::uint32_t avail;
        } __attribute__((packed));

        hardwarecom::Port8Bit MACAddress0Port;
        hardwarecom::Port8Bit MACAddress1Port;
        hardwarecom::Port8Bit MACAddress2Port;
        hardwarecom::Port8Bit MACAddress3Port;
        hardwarecom::Port8Bit MACAddress4Port;
        hardwarecom::Port8Bit MACAddress5Port;

        BufferDescriptor* sendBufferDescr;
        common::uint8_t sendBufferDescrMemory[2048+15];
        common::uint8_t sendBuffers[2*1024+15][8];
        common::uint8_t currentSendBuffer;

        BufferDescriptor* recvBufferDescr;
        common::uint8_t recvBufferDescrMemory[2048+15];
        common::uint8_t recvBuffers[2*1024+15][8];
        common::uint8_t currentRecvBuffer;
        public:
            rtl8169(hardwarecom::PCIDeviceDescriptor* dev,hardwarecom::InterruptManager* interrupts);
            ~rtl8169();
            void Activate();
    };

}

#endif