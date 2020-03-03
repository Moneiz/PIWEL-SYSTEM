#ifndef __SUBPRGM_H
#define __SUBPRGM_H

#include <common/types.h>
#include <utils/string.h>

namespace utils{

    void shell(char buffer[256]);
    void memdump(common::uint32_t start, common::uint16_t size);
}
#endif