#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"
#include "include/driver.h"
#include "mouse.h"

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

class PrintfKeyboardEventHandler : public KeyboardEventHandler{
	public:
	void OnKeyDown(char c){
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

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber){

	
	printf("Initializing GDT...");
	GlobalDescriptorTable gdt;
	printf("Done !");


	InterruptManager interrupts(0x20,&gdt);

	DriverManager driverManager;

		PrintfKeyboardEventHandler kHandler;
		KeyboardDriver keyboard(&interrupts,&kHandler);
		driverManager.AddDriver(&keyboard);

		MouseToConsole mouseHandler;
		MouseDriver mouse(&interrupts, &mouseHandler);
		driverManager.AddDriver(&mouse);

		driverManager.ActivateAll();

		

	interrupts.Activate();
	
	while(1);
}
