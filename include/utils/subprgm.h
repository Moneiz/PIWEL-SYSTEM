#ifndef __SUBPRGM_H
#define __SUBPRGM_H

#include <common/types.h>

namespace utils{

    void shell(char buffer[256]);
    common::uint16_t strcmp(char* str1, char* str2);
    void memdump(common::uint32_t start, common::uint16_t size);
}
#endif