#ifndef __PCI_H
#define __PCI_H

#include <hardwarecom/port.h>
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecom/interrupts.h>

#include <mm.h>

namespace hardwarecom{

    enum BaseAddressRegisterType{
        MemoryMapping = 0,
        InputOutput = 1
    };
    class BaseAddressRegister{
        public:
            bool prefetchable;
            common::uint8_t* address;
            common::uint32_t size;
            BaseAddressRegisterType type;
    };

    class PCIDeviceDescriptor{
        public:
            common::uint32_t portBase;
            common::uint32_t interrupt;

            common::uint16_t bus;
            common::uint16_t device;
            common::uint16_t function;

            common::uint16_t vendor_id;
            common::uint16_t device_id;

            common::uint8_t class_id;
            common::uint8_t subclass_id;
            common::uint8_t interface_id;
            common::uint8_t revision;

            PCIDeviceDescriptor();
            ~PCIDeviceDescriptor();

    };

    class PeripheralComponentInterconnectController{
        hardwarecom::Port32Bit dataPort;
        hardwarecom::Port32Bit commandPort;
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();
            common::uint32_t Read(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint32_t registeroffset);
            void Write(common::uint16_t bus, common::uint16_t device, common::uint16_t function,common::uint32_t registeroffset, common::uint32_t value);
            bool DeviceHasFunction(common::uint16_t bus, common::uint16_t device);

            void SelectDrivers(drivers::DriverManager* driverManager, InterruptManager* interrupt);
            drivers::Driver* GetDriver(PCIDeviceDescriptor dev, InterruptManager* interrupt);
            PCIDeviceDescriptor GetDeviceDescriptor(common::uint16_t bus, common::uint16_t device, common::uint16_t function);
            BaseAddressRegister GetBaseAddressRegister(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint16_t bar);
    };
}

#endif