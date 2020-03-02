#include <utils/subprgm.h>

using namespace common;

static void printf(char*);
static void printfHex(uint8_t);

namespace utils{


    uint16_t strcmp(char* str1, char* str2){
        uint16_t result = 0;
        for(uint16_t i = 0; str1[i] != '\0' || str2[i] != '\0';i++){
            if(str1[i] < str2[i]) result += str2[i] - str1[i];
            else if(str1[i] > str2[i]) result += str1[i] - str2[i];
        }
        return result;
    }
    uint16_t strlen(char* str){
        uint16_t i;
        for(i = 0;str[i] != '\0';i++){}
        return i;
    }

    void memdump(uint32_t start, uint16_t size){

        uint8_t* addrStart = (uint8_t*)start;

        for(uint16_t i = 0; i < size; i++){
            printfHex(*(addrStart+i));
            printf(" ");
        }

    }
    void shell(char buffer[256]){
        if(!strcmp(buffer, "MEMDUMP")){
            memdump(0x00,32);
        }
    }
}