USE16
%include "./inc/x86_types.h"


GLOBAL reset

EXTERN CS_SEL
EXTERN DS_SEL
EXTERN _gdtr
EXTERN start32_launcher
EXTERN VGA_Init


section .ROM_init
start16:
    test eax, 0x0
    jne fault_end

    xor eax, eax
    mov cr3, eax

    mov ax, cs
    mov ds, ax


    mov sp, 0xFFFF   ;Inicializo el stack
    call VGA_Init



    ;Deshabilito la cache
    mov eax, cr0 
    or  eax, (X86_CR0_NW | X86_CR0_CD)
    mov cr0, eax
    wbinvd

    lgdt [_gdtr]    ;Cargo la direccion de la gdt en el registro gdtr

    ; Establecer el micro en Modo Protegido

    smsw ax
    or   ax, X86_CR0_PE
    lmsw ax


    jmp dword CS_SEL:start32_launcher

fault_end:
    hlt
    jmp fault_end

section .resetVector

reset:
    cli
    cld
    jmp start16
halted:
    hlt 
    jmp halted
end: