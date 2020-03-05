#include <utils/subprgm.h>

using namespace common;
using namespace drivers;

static void printf(char*);
static void printfHex(uint8_t);

namespace utils{


    void write(){
        AdvancedTechnologyAttachment ata0m(true, 0x1F0);
        ata0m.Identify();

        AdvancedTechnologyAttachment ata0s(false, 0x1F0);
        ata0s.Identify();
        ata0s.Write28(0,(uint8_t*)"NEANTIS", 7);
        ata0s.Flush();
    }
    void read(){
        AdvancedTechnologyAttachment ata0m(true, 0x1F0);
        ata0m.Identify();

        AdvancedTechnologyAttachment ata0s(false, 0x1F0);
        ata0s.Identify();
        
        ata0s.Read28(0);
        ata0s.Flush();
    }

    void memdump(uint32_t start, uint16_t size){

        uint8_t* addrStart = (uint8_t*)start;

        for(uint16_t i = 0; i < size; i++){
            printfHex(*(addrStart+i));
            printf(" ");
        }

    }
    void shell(char buffer[256]){
        uint32_t params1, params2;
        if(!strcmp(buffer, "MEMDUMP",7)){
            params1 = str8toint32(buffer + 8);
            params2 = str8toint32(buffer + 17);
            printf("entrance in memdump...with ");
            printfHex(params1 >> 24);
            printfHex(params1 >> 16);
            printfHex(params1 >> 8);
            printfHex(params1);
            printf(" size : ");
            printfHex(params2 >> 24);
            printfHex(params2 >> 16);
            printfHex(params2 >> 8);
            printfHex(params2);
            memdump(params1,params2);
        }
        else if(!strcmp(buffer, "WRITE",5)){
            write();
        }
        else if(!strcmp(buffer, "READ",4)){
            read();
        }
    }
}