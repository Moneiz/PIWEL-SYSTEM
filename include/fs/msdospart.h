#ifndef PIWEL_SYSTEM_MSDOSPART_H
#define PIWEL_SYSTEM_MSDOSPART_H

#include <common/types.h>
#include <drivers/ata.h>
#include <fs/fat.h>

namespace fs{

    struct PartitionTableEntry{
        common::uint8_t bootable;

        common::uint8_t start_head;
        common::uint8_t start_sector : 6;
        common::uint16_t start_cylinder : 10;

        common::uint8_t partition_id;

        common::uint8_t end_head;
        common::uint8_t end_sector : 6;
        common::uint16_t end_cylinder : 10;

        common::uint32_t start_lba;
        common::uint32_t length;
    }__attribute__((packed));

    struct MasterBootRecord{
        common::uint8_t bootloader[440];
        common::uint32_t signature;
        common::uint16_t unused;

        PartitionTableEntry primaryPartition[4];

        common::uint16_t magicNumber;
    }__attribute__((packed));

    class MSDOSPartitionTable{

    public:
        static void ReadPartition(drivers::AdvancedTechnologyAttachment *hd);
    };
}

#endif //PIWEL_SYSTEM_MSDOSPART_H
