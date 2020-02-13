#ifndef __VGA_H
#define __VGA_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecom/interrupts.h>
#include <hardwarecom/port.h>

namespace drivers
{
    
    class VideoGraphicsArray{
        protected:
        hardwarecom::Port8Bit miscPort;
        hardwarecom::Port8Bit crtcIndexPort;
        hardwarecom::Port8Bit crtcDataPort;
        hardwarecom::Port8Bit sequencerIndexPort;
        hardwarecom::Port8Bit sequencerDataPort;
        hardwarecom::Port8Bit graphicsControllerIndexPort;
        hardwarecom::Port8Bit graphicsControllerDataPort;
        hardwarecom::Port8Bit attributeControllerIndexPort;
        hardwarecom::Port8Bit attributeControllerPort;
        hardwarecom::Port8Bit attributeControllerWritePort;
        hardwarecom::Port8Bit attributeControllerResetPort;

        void WriteRegisters(common::uint8_t* registers);
        common::uint8_t* GetFrameBufferSegment();

        virtual void PutPixel(common::uint32_t x,common::uint32_t y,common::uint8_t colorIndex);
        virtual common::uint8_t GetColorIndex(common::uint8_t r,common::uint8_t g,common::uint8_t b);
    

        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            virtual bool SupportsMode(common::uint32_t width, common::uint32_t height, common::uint32_t colorDepth);
            virtual bool SetMode(common::uint32_t width, common::uint32_t height, common::uint32_t colorDepth);
            virtual void PutPixel(common::uint32_t x,common::uint32_t y,common::uint8_t r,common::uint8_t g,common::uint8_t b);
    };

}




#endif