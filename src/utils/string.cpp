#include <utils/string.h>

using namespace common;

namespace utils{

    uint16_t strcmp(char* str1, char* str2, int32_t limit){
        uint16_t result = 0;
        for(uint16_t i = 0; (str1[i] != '\0' || str2[i] != '\0') && i < limit ;i++){
            if(str1[i] < str2[i]) result += str2[i] - str1[i];
            else if(str1[i] > str2[i]) result += str1[i] - str2[i];
        }
        return result;
    }
    uint16_t strlen(char* str){
        uint16_t i;
        for(i = 0;str[i];i++){}
        return i;
    }
    /*
    common::uint8_t** split(char* str, char limit){
        uint16_t i;
        uint16_t chunkIndex = 0;
        uint16_t start = 0;
        uint8_t* result[64]; //TODO
        for(i = 0; str[i]; i++){
            if(str[i] == limit){
                result[i] = MemoryManager::activeMemoryManager->malloc(i-start-1);
                start = 0;
            }
        }
        return result;
    }*/
    uint32_t str8toint32(char* str){
        uint32_t result = 0;
        uint8_t byte;
        while(*str){
            byte = *str++;
            if(48 <= byte && byte <= 57)
                byte = byte - 48;
            else if(65 <= byte && byte <= 70) {
                byte = byte - 65 + 10;
            }

            result = (result<<4) | (byte & 0xF);
        }
        return result;
    }
}