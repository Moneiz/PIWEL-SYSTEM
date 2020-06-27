//
// Created by alan on 26/04/2020.
//

#include <fs/msdospart.h>
#include <fs/fat.h>

using namespace fs;
using namespace common;

void printf(char*);
void printfHex(uint8_t);

void MSDOSPartitionTable::ReadPartition(drivers::AdvancedTechnologyAttachment *hd) {

    MasterBootRecord mbr;
    hd->Read28(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));

    /*
    printf("MBR: ");
    for(int i = 0x1BE; i <= 0x1FF ; i++){
        printfHex(((uint8_t*)&mbr)[i]);
        printf(" ");
    }
    printf("\n");

    */

    if(mbr.magicNumber != 0xAA55){
        printf("illegal MBR DISK\n");
        return;
    }

    for(int i = 0 ; i < 4 ; i++){

        if(mbr.primaryPartition[i].partition_id == 0x0){
            continue;
        }

        printf("\nPartition ");
        printfHex(i & 0xFF);
        if(mbr.primaryPartition[i].bootable == 0x80){
            printf(" bootable. Type ");
        }
        else{
            printf(" not bootable. Type ");
        }
        printfHex(mbr.primaryPartition[i].partition_id);

        ReadBIOSBlock(hd, mbr.primaryPartition[i].start_lba);
    }

}