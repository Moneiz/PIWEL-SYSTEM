#ifndef __DRIVER_H
#define __DRIVER_H

namespace drivers{

    class Driver{
        public:
            Driver();
            ~Driver();

            virtual void Activate();
            virtual int Reset();
            virtual void Deactivate();
            
    };
    class DriverManager{
        protected:
            Driver* drivers[265];
            int numDrivers;
        public:
            DriverManager();
            void AddDriver(Driver* d);
            void ActivateAll();
    };
}

#endif