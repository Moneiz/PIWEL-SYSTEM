#ifndef __ATA_H
#define __ATA_H

#include <common/types.h>
#include <hardwarecom/interrupts.h>
#include <hardwarecom/port.h>

namespace drivers{
    class AdvancedTechnologyAttachment{
        bool master;
        hardwarecom::Port16Bit dataPort;
        hardwarecom::Port8Bit errorPort;
        hardwarecom::Port8Bit sectorCountPort;
        hardwarecom::Port8Bit lbaLowPort;
        hardwarecom::Port8Bit lbaMidPort;
        hardwarecom::Port8Bit lbaHiPort;
        hardwarecom::Port8Bit devicePort;
        hardwarecom::Port8Bit commandPort;
        hardwarecom::Port8Bit controlPort;
        public:
        AdvancedTechnologyAttachment(bool master, common::uint16_t portBase);
        ~AdvancedTechnologyAttachment();
        void Identify();
        void Read28(common::uint32_t sectorNum, int count = 512);
        void Write28(common::uint32_t sectorNum, common::uint8_t* data, common::uint32_t count);
        void Flush();
    };
}

#endif