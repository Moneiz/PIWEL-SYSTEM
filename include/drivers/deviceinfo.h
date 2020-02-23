#ifndef __DEVICE_INFO_H
#define __DEVICE_INFO_H

#include <common/types.h>

namespace drivers{
    class DeviceInfo{
        public:
        static const char* VENDORLIST[64] = {
            "Intel Corp.",
            "InnoTek Systemberatung GmbH",
            "AMD Inc.",
            "Apple Inc."
        };
        static const char* DEVICELIST[64] = {
            "440FX",
            "82371SB PIIX3 ISA",
            "82371AB/EB/MB PIIX4 IDE",
            "VirtualBox Graphics Adapter",
            "79c970",
            "VirtualBox Guest Service",
            "82801AA AC'97 Audio Contr.",
            "KeyLargo/Intrepid USB",
            "82371AB/EB/MB PIIX4 ACPI"
        };
        static common::uint16_t VEN_DEV_ASS[4*9] = {
            0X8086,0X1237, 0,0,
            0X8086,0X7000, 0,1,
            0X8086,0X7111, 0,2,
            0X80ee,0XBEEF, 1,3,
            0X1022,0X2000, 2,4,
            0X80ee,0XCAFE, 1,5,
            0X8086,0X2415, 0,6,
            0X106B,0X003F, 3,7,
            0X8086,0X7113, 0,8
        };
        DeviceInfo(common::uint16_t vendor, common::uint16_t device);
        ~DeviceInfo();
        void showInfo();
         
    };
}

#endif