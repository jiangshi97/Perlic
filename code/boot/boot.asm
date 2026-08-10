[section .multiboot2]
align 8
head_start:
    dd magic_num
    dd i386
    dd head_length
    dd sum_check
align 8
    dw 3    ;start address
    dw 0
    dd 12
    dd (_start - 0xffff800000000000)
align 8
    dw 5    ;set vedio mode
    dw 1
    dd 20
    dd 1024
    dd 768
    dd 32
align 8
    dw 0
    dw 0
    dd 8
head_end:


[section .text]

global _start
global ignore_int
global IDT_POINTER

BITS 32

_start:     ;The beginning of kernel
    mov [eax_magicnum - 0xffff800000000000], eax ;Save grub structure
    mov [ebx_bootinf - 0xffff800000000000], ebx 

    lgdt [GDT_POINTER - 0xffff800000000000]  ;load GDT and IDT
    lidt [IDT_POINTER - 0xffff800000000000]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax
    mov esp, 0x7E00

    mov eax, cr4    ;enable PAE
    bts eax, 5
    mov cr4, eax

    ;lea eax, [_PML4E - 0xffff800000000000]
    mov eax, _PML4E - 0xffff800000000000;load page table
    mov cr3, eax

    mov ecx, 0C0000080h
    rdmsr
    bts eax, 8
    wrmsr
a1:

    mov eax, cr0
    bts eax, 0
    bts eax, 31
    mov cr0, eax
    
a2:
BITS 32
    push dword 0x08
    mov eax, te - 0xffff800000000000 
    push eax
    retf

BITS 64
te:
    jmp entry64
entry64:
    lgdt [rel GDT_POINTER]  ;load GDT and IDT
    lidt [rel IDT_POINTER]

    mov rax, 0x10
    mov ds, rax
    mov es, rax
    mov gs, rax
    mov ss, rax
    mov rsp, 0xffff800000007e00 ;temporary stack
Setup_IDT:
    lea rdx, [rel ignore_int]
    mov rax, (0x08 << 16)
    mov ax, dx
    mov rcx, (0x8e00 << 32)
    add rax, rcx
    mov ecx, edx
    shr ecx, 16
    shl rcx, 48
    add rax, rcx
    shr rdx, 32
    lea rdi, [rel IDT_Table]
    mov rcx, 256
rp_sidt:
    mov [rdi], rax
    mov [rdi + 8], rdx
    add  rdi, 0x10
    dec rcx
    jne rp_sidt
Setup_TSS64:
    lea rdx, [rel TSS64_Table]
    xor rax, rax
    xor rcx, rcx
    mov rax, 0x89
    shl rax, 40
    mov ecx, edx
    shr ecx, 24
    shl rcx, 56
    add rax, rcx
    xor rcx, rcx
    mov ecx, edx
    and ecx, 0xffffff
    shr rcx, 16
    add rax, rcx
    add rax, rcx
    add rax, 103
    lea rdi, [rel GDT_Table]
    mov [rdi + 80], rax
    shr rdx, 32
    mov [rdi + 88], rdx

extern KernelMain
    jmp KernelMain
Loop2:
    hlt
    jmp Loop2


ignore_int:
    cld
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    call [rel prt]
    ;this should call printk
Loop1:
    hlt
    jmp Loop1

a3:
    ret
magic_num   equ 0xe85250d6
i386    equ 0
head_length equ (head_end - head_start)
sum_check equ -(magic_num + i386 + head_length)

[section .data]

align 8
global eax_magicnum
global ebx_bootinf
global string_buff
global prt
eax_magicnum:
    dq 0

ebx_bootinf:
    dq 0

string_buff:
    times 512 dq 0

prt:
    dq a3

global GDT_Table
GDT_Table:  ;temporary GDT table
    dq 0x0000000000000000
    dq 0x0020980000000000
    ;dq 0x0000920000000000
    dq 0x00cf92000000ffff
    dq 0x0020f80000000000
    dq 0x0000f20000000000
    dq 0x00cf9a000000ffff
    dq 0x00cf92000000ffff
    times 10 dq 0
GDT_End:
GDT_POINTER:
GDT_LIMIT:
    dw GDT_End - GDT_Table - 1
GDT_BASE:
    dq GDT_Table

align 8
global IDT_Table
IDT_Table:  ;temporary IDT table
    times 512 dq 0
IDT_END:
IDT_POINTER:
IDT_LIMIT:
    dw IDT_END - IDT_Table - 1
IDT_BASE:
    dq IDT_Table

align 8
global TSS64_Table
TSS64_Table:    ;temporary TSS table
    times 13 dq 0
TSS64_END:
TSS64_POINTER:
TSS64_LIMIT:
    dw TSS64_END - TSS64_Table - 1
TSS64_BASE:
    dq TSS64_Table

align 0x1000
global _PML4E
global _PDPTE
global _PDE
_PML4E: ;temporary page table
    dq (_PDPTE + 7 - 0xffff800000000000)
    times 255 dq 0
    dq (_PDPTE2 + 7 - 0xffff800000000000)
    times 255 dq 0

align 0x1000
_PDPTE:
    dq (_PDE + 3 - 0xffff800000000000)
    times 511 dq 0

align 0x1000
_PDE:
    dq 0x000083
    dq 0x200083
    dq 0x400083
    dq 0x600083
    dq 0x800083
    dq 0xa00083
    dq 0xc00083
    dq 0xe00083
    dq 0x1000083
    dq 0x1200083
    dq 0x1400083
    dq 0x1600083
    dq 0x1800083
    times 499 dq 0
align 0x1000
_PDPTE2:
    dq (_PDE2 + 3 - 0xffff800000000000)
    times 511 dq 0

align 0x1000
_PDE2:
    dq 0x000083
    dq 0x200083
    dq 0x400083
    dq 0x600083
    dq 0x800083
    dq 0xa00083
    dq 0xc00083
    dq 0xe00083
    dq 0x1000083
    dq 0x1200083
    dq 0x1400083
    dq 0x1600083
    dq 0x1800083
    times 499 dq 0