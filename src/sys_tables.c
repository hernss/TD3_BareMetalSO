#include "../inc/prototipos.h"

extern dword kernel32_init;
extern dword __PILA_KERNEL;

typedef struct 
{
    /* data */
    word limit;
    word base0;
    byte base1;
    byte attributes;
    byte limit_attributes;
    byte base2;
} item_gdt;




#pragma pack(2)
__attribute__(( section(".systable_c"))) item_gdt gdt[6];
__attribute__(( section(".systable_c"))) struct {
    word limit;
    void *base_address;
} _gdtr_c;


#pragma pack(2)
__attribute__(( section(".systable_c"))) st_trueTSS trueTSS;

__attribute__(( section(".functions_rom"))) void create_tss(){
    //Inicializo todo con 0 la estructura de TSS
    for(word i=0;i<sizeof(trueTSS); i++){
        byte *p = (byte*)&trueTSS + i;
        *p = 0;
    }

    trueTSS.CS = CS_SEL_C;
    trueTSS.DS = DS_SEL_C;
    trueTSS.ES = DS_SEL_C;
    trueTSS.FS = DS_SEL_C;
    trueTSS.GS = DS_SEL_C;
    trueTSS.SS = DS_SEL_C;

    trueTSS.EFLAGS = 0x202;

    trueTSS.EIP = (dword)&kernel32_init;

    trueTSS.CR3 = 0x00010000;   //Kernel CR3

    trueTSS.SS0 = DS_SEL_C;
    trueTSS.ESP0 = 0x00792FFF; //((dword)&__PILA_KERNEL) + 0xFFF;
    
    return;
}

__attribute__(( section(".functions_rom"))) byte create_gdt(){


    //Descriptor nulo
    gdt[0].limit = 0;
    gdt[0].base0 = 0;
    gdt[0].base1 = 0;
    gdt[0].attributes = 0;
    gdt[0].limit_attributes = 0;
    gdt[0].base2 = 0;

    //CS_SEL
    gdt[CS_SEL_C/8].limit = 0xFFFF;
    gdt[CS_SEL_C/8].base0 = 0x0000;
    gdt[CS_SEL_C/8].base1 = 0;
    gdt[CS_SEL_C/8].attributes = 0x99;
    gdt[CS_SEL_C/8].limit_attributes = 0xCF;
    gdt[CS_SEL_C/8].base2 = 0;

    //DS_SEL
    gdt[DS_SEL_C/8].limit = 0xFFFF;
    gdt[DS_SEL_C/8].base0 = 0x0000;
    gdt[DS_SEL_C/8].base1 = 0;
    gdt[DS_SEL_C/8].attributes = 0x93;
    gdt[DS_SEL_C/8].limit_attributes = 0xCF;
    gdt[DS_SEL_C/8].base2 = 0;

    //Cargo los datos por defecto de la TSS
    create_tss();
    //TSS
    gdt[TSS_SEL_C/8].limit = 0x0067;
    gdt[TSS_SEL_C/8].base0 = (word)&trueTSS;
    gdt[TSS_SEL_C/8].base1 = (byte)(((dword)&trueTSS)>>16);
    gdt[TSS_SEL_C/8].attributes = 0x89;
    gdt[TSS_SEL_C/8].limit_attributes = 0x40;
    gdt[TSS_SEL_C/8].base2 = (byte)(((dword)&trueTSS)>>24);

    //CS Selector USER
    gdt[CS_SEL_C_USER/8].limit = 0xFFFF;
    gdt[CS_SEL_C_USER/8].base0 = 0x0000;
    gdt[CS_SEL_C_USER/8].base1 = 0;
    gdt[CS_SEL_C_USER/8].attributes = 0xF9;
    gdt[CS_SEL_C_USER/8].limit_attributes = 0xCF;
    gdt[CS_SEL_C_USER/8].base2 = 0;


    //DS Selector USER
    gdt[DS_SEL_C_USER/8].limit = 0xFFFF;
    gdt[DS_SEL_C_USER/8].base0 = 0x0000;
    gdt[DS_SEL_C_USER/8].base1 = 0;
    gdt[DS_SEL_C_USER/8].attributes = 0xF3;
    gdt[DS_SEL_C_USER/8].limit_attributes = 0xCF;
    gdt[DS_SEL_C_USER/8].base2 = 0;
    
    _gdtr_c.limit = sizeof(gdt);
    _gdtr_c.base_address = &gdt;
    return 1;
}