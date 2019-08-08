void printf(char* str){

	unsigned short* video_memory = (unsigned short*) 0xB8000;
	for(int i = 0; str[i] != '\0';i++){
		video_memory[i] = (video_memory[i]& 0xFF00) |  str[i];
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

extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber){

	printf("Hello World!");

	while(1);
}
