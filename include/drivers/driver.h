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
            
            int numDrivers;
        public:
            Driver* drivers[265];
            DriverManager();
            void AddDriver(Driver* d);
            void ActivateAll();
    };
}

#endif