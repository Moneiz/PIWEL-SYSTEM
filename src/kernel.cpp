#include <common/types.h>
#include <gdt.h>
#include <mm.h>
#include <scheduling.h>
#include <hardwarecom/interrupts.h>
#include <hardwarecom/pci.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/driver.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <utils/subprgm.h>
#include <drivers/amd_am79c973.h>
#include <syscalls.h>
#include <net/etherframe.h>
#include <net/arp.h>
#include <net/ipv4.h>
#include <net/icmp.h>
#include <net/udp.h>

//#define GRAPHMODE

using namespace common;
using namespace drivers;
using namespace hardwarecom;
using namespace gui;
using namespace net;

int main();

char buffer[256];
uint8_t indexBuffer=0;

void printf(char* str){

	uint16_t* video_memory = (uint16_t*) 0xB8000;
	
	static uint8_t x=0,y=0;

	for(int i = 0; str[i] != '\0';i++){
		
		switch (str[i])
		{
		case '\n':
			x=0;
			y++;
			break;
		case 0x7F:
			if(indexBuffer > 0){
				x--;
				buffer[--indexBuffer] = '\0';
				if(x > 128){ //ouai un peu spécial
					x = 0;
				}
				video_memory[80*y+x] = (video_memory[80*y+x] & 0xFF00) | ' ';
			}
			break;
		default:
			video_memory[80*y+x] = (video_memory[80*y+x]& 0xFF00) |  str[i];
			x++;
			break;
		}
		
		

		if(x >= 80){
			y++;
			x = 0;
		}
		if(y >= 25){

			for(y = 0; y < 25;y++){
				for(x = 0; x < 80; x++){
					video_memory[80*y+x] = (video_memory[80*y+x] & 0xFF00) | ' ';
				}
			}

			x = 0;
			y = 0;
		}
	}

}
void printfHex(uint8_t key){
	char* foo = "00";
	char* hex = "0123456789ABCDEF";
	foo[0] = hex[(key>>4) & 0xF];
	foo[1] = hex[(key&0xF)];
	printf(foo);
}

class PrintfUDPHandler : public UserDatagramProtocolHandler{
public:
    void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket* socket, common::uint8_t* data, common::uint16_t size){
        char* foo = " ";
        for(int i = 0; i < size; i++){
            foo[0] = data[i];
            printf(foo);
        }
    }
};

void sysprintf(char* str){
	asm("int $0x80" : : "a" (4), "b" (str));
}

void TaskA(){
	while(true){
		sysprintf("A");
	}
}
void TaskB(){
	while(true){
		sysprintf("B");
	}
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler{
	public:
	void OnKeyDown(char c){
		if(c == '\n'){
			indexBuffer = 0;
			utils::shell(buffer);
			buffer[0] = '\0';
		}else if(c == 0x7F){
		}
		else
		{
			buffer[indexBuffer++] = c;
			buffer[indexBuffer] = '\0';
		}
		char* foo = " ";
		foo[0] = c;
		printf(foo);
	}
};

class MouseToConsole : public MouseEventHandler{
	int8_t x,y;
	public:
	MouseToConsole(){

	}
	virtual void OnActivate(){
		uint16_t* VideoM = (uint16_t*)0xb8000;
		x = 40;
		y = 12;
		VideoM[80*y+x] = (VideoM[80*y+x] & 0x0F00) << 4
			|(VideoM[80*y+x] & 0xF000) >> 4
			|(VideoM[80*y+x] & 0x00FF);
	}
	virtual void OnMouseMove(int xoffset, int yoffset){
		static uint16_t* VideoM = (uint16_t*)0xb8000;
		
		VideoM[80*y+x] = (VideoM[80*y+x] & 0x0F00) << 4
			|(VideoM[80*y+x] & 0xF000) >> 4
			|(VideoM[80*y+x] & 0x00FF);

		x += xoffset;

		if(x >= 80) x = 79;
		if(x < 0) x = 0;
		y +=yoffset;
		if(y >= 25) y = 24;
		if(y < 0) y = 0;

		VideoM[80*y+x] = (VideoM[80*y+x] & 0x0F00) << 4
			|(VideoM[80*y+x] & 0xF000) >> 4
			|(VideoM[80*y+x] & 0x00FF);
	}
};

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors(){
	for(constructor* i = &start_ctors; i != &end_ctors; i++){
		(*i)();
	}
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t){

	
	printf("Initializing GDT...");
	GlobalDescriptorTable gdt;
	
	uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
	size_t heap = 10*1024*1024;
	MemoryManager memoryManager(heap,(*memupper)*1024 - heap - 10*1024);

	printf("heap: 0x");
	printfHex((heap >> 24) & 0xFF);
	printfHex((heap >> 16) & 0xFF);
	printfHex((heap >> 8) & 0xFF);
	printfHex((heap ) & 0xFF);
	printf("\n");

	TaskManager taskManager;
	
	/*
	Task task1(&gdt, TaskA);
	Task task2(&gdt, TaskB);
	taskManager.AddTask(&task1);
	taskManager.AddTask(&task2);
	*/
	InterruptManager interrupts(0x20, &gdt, &taskManager);
	SyscallHandler syscalls(&interrupts,0x80);

	#ifdef GRAPHMODE
		Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
	#endif
	DriverManager driverManager;

		#ifdef GRAPHMODE
			KeyboardDriver keyboard(&interrupts,&desktop);
		#else
			PrintfKeyboardEventHandler kHandler;
			KeyboardDriver keyboard(&interrupts, &kHandler);
		#endif
		driverManager.AddDriver(&keyboard);

		#ifdef GRAPHMODE
			MouseDriver mouse(&interrupts, &desktop);
		#else
			MouseToConsole mouseHandler;
			MouseDriver mouse(&interrupts, &mouseHandler);
		#endif
		driverManager.AddDriver(&mouse);

		PeripheralComponentInterconnectController PCIController;
		PCIController.SelectDrivers(&driverManager, &interrupts);

		#ifdef GRAPHMODE
			VideoGraphicsArray vga;
		#endif

		driverManager.ActivateAll();

	#ifdef GRAPHMODE
		vga.SetMode(320,200,8);
		Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
		desktop.AddChild(&win1);
		Window win2(&desktop, 40, 15,30,30,0x00, 0xA8, 0x00);
		desktop.AddChild(&win2);
    #endif

	uint8_t ip1 = 10, ip2 = 0, ip3 = 2, ip4 = 15;
	uint32_t ip_be = ((uint32_t) ip4 << 24)
	        | ((uint32_t)ip3 << 16)
	        | ((uint32_t)ip2 << 8)
	        | ((uint32_t)ip1);

    uint8_t gip1 = 10, gip2 = 0, gip3 = 2, gip4 = 1;
    uint32_t gip_be = ((uint32_t) gip4 << 24)
                     | ((uint32_t)gip3 << 16)
                     | ((uint32_t)gip2 << 8)
                     | ((uint32_t)gip1);

	amd_am79c973* eth0 = (amd_am79c973*)(driverManager.drivers[2]);
	eth0->SetIPAddress(ip_be);

	EtherFrameProvider etherFrame(eth0);

	AddressResolutionProtocol arp(&etherFrame);

    uint8_t subnet1 = 255, subnet2 = 255, subnet3 = 255, subnet4 = 0;
    uint32_t subnet_be = ((uint32_t) subnet4 << 24)
                      | ((uint32_t)subnet3 << 16)
                      | ((uint32_t)subnet2 << 8)
                      | ((uint32_t)subnet1);

	InternetProtocolProvider ipv4(&etherFrame,&arp,gip_be,subnet_be);
    InternetControlMessageProtocol icmp(&ipv4);
    UserDatagramProtocolProvider udp(&ipv4);

	interrupts.Activate();

	arp.BroadcasMACAddress(gip_be);
	icmp.RequestEchoReply(gip_be);

	PrintfUDPHandler udphandler;
	UserDatagramProtocolSocket* udpsocket = udp.Listen(1234);
	udp.Bind(udpsocket, &udphandler);

	while(1){
		#ifdef GRAPHMODE
			desktop.Draw(&vga);
		#endif
	}
}