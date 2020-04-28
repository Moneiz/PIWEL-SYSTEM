#include <fs/fat.h>

using namespace fs;
using namespace drivers;
using namespace common;

void printf(char* str);
void printfHex(uint8_t);

void fs::ReadBIOSBlock(AdvancedTechnologyAttachment *hd, uint32_t partitionOffset){
    BiosParameterBlock32 bpb;
    hd->Read28(partitionOffset, (uint8_t*)&bpb, sizeof(BiosParameterBlock32));

    /*
    for(int i = 0x00; i < sizeof(BiosParameterBlock32); i++){
        printfHex(((uint8_t*)&bpb)[i]);
        printf(" ");
    }
    printf("\n");
    */

    uint32_t fatStart = partitionOffset + bpb.reservedSectors;
    uint32_t fatSize = bpb.tableSize;
    uint32_t dataStart = fatStart + fatSize * bpb.fatCopies;

    uint32_t rootStart = dataStart + bpb.sectorsPerCluster*(bpb.rootCluster-2);

    DirectoryEntryFat32 dirent[16];
    hd->Read28(rootStart,(uint8_t*)&dirent[0], 16* sizeof(DirectoryEntryFat32));

    for(int i = 0; i < 16; i++){
        if(dirent[i].name[0] == 0x00){
            break;
        }
        if((dirent[i].attributes & 0x0F) == 0x0F){
            continue;
        }
        char* foo="        ";
        for(int j = 0x00; j < 8; j++){
            foo[j] = dirent[i].name[j];
        }
        printf(foo);
        printf("\n");

        if((dirent[i].attributes & 0x10) == 0x10){ //directory
            continue;
        }

        uint32_t firstFileSector = ((uint32_t)dirent[i].firstClusterHi) << 16
        | ((uint32_t)dirent[i].firstClusterLow);

        int32_t SIZE = dirent[i].size;
        int32_t nextFileCluster = firstFileSector;

        uint8_t buffer[513];
        uint8_t fatBuffer[513];
        while(SIZE > 0) {
            uint32_t fileSector = dataStart + bpb.sectorsPerCluster * (nextFileCluster - 2);

            int sectorOffset = 0;
            for (; SIZE > 0; SIZE -= 512) {
                hd->Read28(fileSector + sectorOffset, buffer, 512);
                sectorOffset++;

                if(++sectorOffset > bpb.sectorsPerCluster-1){
                    break;
                }

                buffer[SIZE > 512 ? 512 : SIZE] = '\0';
                printf((char *) buffer);
            }
            uint32_t fatSectorForCurrentCluster = nextFileCluster / (512/ sizeof(uint32_t));
            hd->Read28(fatStart+fatSectorForCurrentCluster, fatBuffer, 512);

            uint32_t fatOffsetInSectorForCurrentCluster = nextFileCluster % (512/ sizeof(uint32_t));


            nextFileCluster = ((uint32_t*)&fatBuffer)[fatOffsetInSectorForCurrentCluster] & 0x0fffffff;
        }



    }
}