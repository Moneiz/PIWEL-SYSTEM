#ifndef __STRING_H
#define __STRING_H

#include <common/types.h>
#include <mm.h>

namespace utils{

    common::uint16_t strcmp(char* str1, char* str2, common::int32_t limit);
    common::uint16_t strlen(char* str);
    common::uint32_t str8toint32(char* str);
    //common::uint8_t** split(char* str, char limit);
}

#endif