GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
		obj/gdt.o \
		obj/mm.o \
		obj/drivers/driver.o \
		obj/hardwarecom/port.o \
		obj/hardwarecom/interruptstubs.o \
		obj/hardwarecom/interrupts.o \
		obj/syscalls.o \
		obj/scheduling.o \
		obj/drivers/amd_am79c973.o \
		obj/drivers/rlt8169.o\
		obj/hardwarecom/pci.o \
		obj/drivers/keyboard.o \
		obj/drivers/mouse.o \
		obj/drivers/vga.o \
		obj/drivers/ata.o \
		obj/fs/msdospart.o \
		obj/fs/fat.o \
		obj/gui/widget.o \
		obj/gui/window.o \
		obj/gui/desktop.o \
		obj/utils/string.o \
		obj/utils/subprgm.o \
		obj/net/etherframe.o \
		obj/net/arp.o \
		obj/net/ipv4.o \
		obj/net/icmp.o \
		obj/net/udp.o \
		obj/net/tcp.o \
		obj/kernel.o

run: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm "MY OS" &

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin
mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "MY OS" {' >> iso/boot/grub/grub.cfg
	echo ' multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo ' boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso



.PHONY: clean
clean:
	rm -rf obj mykernel.bin mykernel.iso