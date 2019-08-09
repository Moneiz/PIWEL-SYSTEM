#include "types.h"
#include "gdt.h"
#include "interrupts.h"

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

	InterruptManager interrupts(&gdt);



	interrupts.Activate();
	
	while(1);
}
