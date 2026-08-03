
OUTPUT_FORMAT("elf64-x86-64","elf64-x86-64","elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)


SECTIONS
{

	 . = 0xffff800000000000 + 0x100000;    

    .multiboot2 :	AT(0x100000)
	{
		. = ALIGN(8);
		_multiboot2 = .;
		KEEP(*(.multiboot2))
		_emultiboot2 = .;
	}

	.text :
	{
		. = ALIGN(4);
		_text = .;
		*(.multiboot)
		*(.text)
		_etext = .;
	}

	. = ALIGN(8);

	.data :
	{
		. = ALIGN(4); 
		_data = .;
		*(.data)		
		_edata = .;
	}

	.rodata : 
	{
		. = ALIGN(4); 
		_rodata = .;	
		*(.rodata)
		_erodata = .;
	}

	. = ALIGN(32768);
	.data.init_task : { *(.data.init_task) }

	.bss (NOLOAD) :
	{
		. = ALIGN(4); 
		_bss = .;
		*(.bss)
		_ebss = .;
	}


	_end = .;
}

