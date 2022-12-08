#include "../inc/prototipos.h"

typedef struct 
{
    /* data */
    word offset1;
    word segment;
    word attributes;
    word offset2;
} item_idt;

#define IDT_LENGTH 256

#pragma pack(2)
__attribute__(( section(".interrupt_table_c"))) item_idt idt[IDT_LENGTH];
__attribute__(( section(".interrupt_table_c"))) struct {
    word limit;
    void *base_address;
} _idtr_c;

__attribute__(( section(".functions_rom"))) byte create_idt(){

    for (word i=0; i< IDT_LENGTH; i++){
        idt[i].segment = CS_SEL_C;
        idt[i].offset1 = (word)(&default_interrupt);
        idt[i].offset2 = ((dword)(&default_interrupt))>>16;
        idt[i].attributes = 0x8F00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0
    }
    
    idt[6].segment = CS_SEL_C;
    idt[6].offset1 = (word)(&handler_UD);
    idt[6].offset2 = ((dword)(&handler_UD))>>16;
    idt[6].attributes = 0x8F00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0

    idt[7].segment = CS_SEL_C;
    idt[7].offset1 = (word)(&handler_NM);
    idt[7].offset2 = ((dword)(&handler_NM))>>16;
    idt[7].attributes = 0x8F00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0

    idt[8].segment = CS_SEL_C;
    idt[8].offset1 = (word)(&handler_DF);
    idt[8].offset2 = ((dword)(&handler_DF))>>16;
    idt[8].attributes = 0x8F00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0

    idt[12].segment = CS_SEL_C;
    idt[12].offset1 = (word)(&handler_SS);
    idt[12].offset2 = ((dword)(&handler_SS))>>16;
    idt[12].attributes = 0x8F00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0

    idt[13].segment = CS_SEL_C;
    idt[13].offset1 = (word)(&handler_GP);
    idt[13].offset2 = ((dword)(&handler_GP))>>16;
    idt[13].attributes = 0x8F00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0

    idt[14].segment = CS_SEL_C;
    idt[14].offset1 = (word)(&handler_PF);
    idt[14].offset2 = ((dword)(&handler_PF))>>16;
    idt[14].attributes = 0x8F00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0

    idt[17].segment = CS_SEL_C;
    idt[17].offset1 = (word)(&handler_AC);
    idt[17].offset2 = ((dword)(&handler_AC))>>16;
    idt[17].attributes = 0x8F00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0

    idt[32].segment = CS_SEL_C;
    idt[32].offset1 = (word)(&handler_PIT0);
    idt[32].offset2 = ((dword)(&handler_PIT0))>>16;
    idt[32].attributes = 0x8E00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0

    idt[33].segment = CS_SEL_C;
    idt[33].offset1 = (word)(&handler_teclado);
    idt[33].offset2 = ((dword)(&handler_teclado))>>16;
    idt[33].attributes = 0x8E00;   //P=1 DPL=00 S=0 D=1 TIPO=111(Excepcion) el resto en 0

    idt[0x80].segment = CS_SEL_C;
    idt[0x80].offset1 = (word)(&handler_INT80);
    idt[0x80].offset2 = ((dword)(&handler_INT80))>>16;
    idt[0x80].attributes = 0xEE00;   //P=1 DPL=11 S=0 D=1 TIPO=110(Interrupcion) el resto en 0

    _idtr_c.limit = sizeof(idt);
    _idtr_c.base_address = &idt;
    return 1;
}