#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <common/types.h>
#include <hardwarecom/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecom/port.h>

namespace drivers{

    class KeyboardEventHandler{
        public:
            KeyboardEventHandler();
            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
    };

    class KeyboardDriver : public hardwarecom::InterruptHandler, public Driver{

        hardwarecom::Port8Bit dataport;
        hardwarecom::Port8Bit commandport;

        KeyboardEventHandler* handler;
        public:
            KeyboardDriver(hardwarecom::InterruptManager* manager,KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual void Activate();
            virtual common::uint32_t HandlerInterrupt(common::uint32_t esp);

    };
}

#endif