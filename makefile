build:./build/kernel
	cp ./build/kernel ./target/live
	sudo grub-mkrescue -o ./build/my_os.iso ./target
run:
	make build
	qemu-system-x86_64 -serial stdio -cdrom ./build/my_os.iso
./build/kernel:./code/boot/boot.o ./code/boot/bootc.o ./code/debug/sprint.o ./code/debug/serial.o ./code/kernel/trap.o ./code/kernel/traps.o ./code/kernel/gates.o ./code/debug/test.o
	ld $^ -T ./code/kernel.lds -o ./build/kernel
./code/boot/boot.o:./code/boot/boot.asm
	nasm -o $@ $< -f elf64
%.o: %.asm
	nasm -o $@ $< -f elf64
%.o: %.c
	gcc  -mcmodel=large -fno-stack-protector -fno-builtin -fno-strict-aliasing -m64 -I ./code/include -c -o $@ $<



