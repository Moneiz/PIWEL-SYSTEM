#ifndef __MOUSE_H
#define __MOUSE_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecom/interrupts.h>
#include <hardwarecom/port.h>

namespace drivers{
    class MouseEventHandler{
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(common::uint8_t button);
            virtual void OnMouseUp(common::uint8_t button);
            virtual void OnMouseMove(int x,int y);

    };

    class MouseDriver : public hardwarecom::InterruptHandler, public Driver{

        hardwarecom::Port8Bit dataport;
        hardwarecom::Port8Bit commandport;
        common::uint8_t buffer[3];
        common::uint8_t offset;
        common::uint8_t buttons;

        MouseEventHandler* handler;

        public:
            MouseDriver(hardwarecom::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual void Activate();
            virtual common::uint32_t HandlerInterrupt(common::uint32_t esp);

    };
}

#endif