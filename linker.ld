
ENTRY (reset)


/*
    Organizacion de la bios
    0xFFFF0000          init16
                        init32
    
    0xFFFF0300          systables_rom
    0xFFFF0600          gdt base (ROM)
                        functions_rom
    0xFFFF1DEE          datos
    0xFFFF1F95          datos_user
    0xFFFF1FBD          mmx _data
    0xFFFF27C1          utils16
    0xFFFF3AD1

    0xFFFF4000          __SYS_TABLES_C_LMA (tabla gdt y idt que se copia a la RAM)
                        trueTSS
                        interrupts
    0xFFFF524B          codigo_kernel32
                        functions_ram
    0xFFFF6D15(aprox)

    0xFFFF7000          tarea_1_text
                        tarea_1_bss
                        tarea_1_data
                        tarea_1_rodata
    0xFFFF751F          tarea_2_text
                        tarea_2_bss
                        tarea_2_data
                        tarea_2_rodata
    0xFFFF7624          tarea_3_text
                        tarea_3_bss
                        tarea_3_data
                        tarea_3_rodata
    0xFFFF7729          tarea_4_text
                        tarea_4_bss
                        tarea_4_data
                        tarea_4_rodata
    0xFFFF7733
    
    
    0xFFFFFFF0          vector de reset

*/
    __STACK_START_32        = 0x1FFF8000;
    __STACK_END_32          = 0x1FFF8FFF;
    __STACK_SIZE_32         = ((__STACK_END_32 - __STACK_START_32) / 4);   /* Tamaño en unidades de 32b*/

  
    /*****************FISICAS*******************/
    __VIDEO_RAM_PHI         = 0x000B8000;
    __INTERRUPTS_PHI        = 0x00100000;  
    __DATOS_PHI             = 0x00200000;
    __DATOS_USER_PHI        = 0x00201000;
    __DIGITOS_PHI           = 0x00210000;
    __KERNEL_32_PHI         = 0x00220000;

    __MMX_DATA_T1_PHI       = 0x00301000;
    __CONTEXT_PAGE_TAREA_1_PHI = 0x00300000;
    __TAREA_1_TEXT_PHI      = 0x00310000;
    __TAREA_1_BSS_PHI       = 0x00320000;
    __TAREA_1_DATA_PHI      = 0x00330000;
    __TAREA_1_RODATA_PHI    = 0x00340000;

    __MMX_DATA_T2_PHI       = 0x00401000;
    __CONTEXT_PAGE_TAREA_2_PHI = 0x00400000;
    __TAREA_2_TEXT_PHI      = 0x00410000;
    __TAREA_2_BSS_PHI       = 0x00420000;
    __TAREA_2_DATA_PHI      = 0x00430000;
    __TAREA_2_RODATA_PHI    = 0x00440000;

    __MMX_DATA_T3_PHI       = 0x00501000;
    __CONTEXT_PAGE_TAREA_3_PHI = 0x00500000;
    __TAREA_3_TEXT_PHI      = 0x00510000;
    __TAREA_3_BSS_PHI       = 0x00520000;
    __TAREA_3_DATA_PHI      = 0x00530000;
    __TAREA_3_RODATA_PHI    = 0x00540000;

    __MMX_DATA_T4_PHI       = 0x00601000;
    __CONTEXT_PAGE_TAREA_4_PHI = 0x00600000;
    __TAREA_4_TEXT_PHI      = 0x00610000;
    __TAREA_4_BSS_PHI       = 0x00620000;
    __TAREA_4_DATA_PHI      = 0x00630000;
    __TAREA_4_RODATA_PHI    = 0x00640000;

    __RAMDOM_PAGE_START     = 0x0A000000;

    __PILA_KERNEL_PHI       = 0x1FFF8000;
    __TAREA_1_PILA_PHI      = 0x2FFFF000;
    __TAREA_2_PILA_PHI      = 0x30000000;
    __TAREA_3_PILA_PHI      = 0x30001000;
    __TAREA_4_PILA_PHI      = 0x30002000;

    __TAREA_1_PILA_KERNEL_PHI      = 0x2FFF4000;
    __TAREA_2_PILA_KERNEL_PHI      = 0x2FFF5000;
    __TAREA_3_PILA_KERNEL_PHI      = 0x2FFF6000;
    __TAREA_4_PILA_KERNEL_PHI      = 0x2FFF7000;

    
    __TAREA_1_CR3           = 0x00800000; /*DPT + 255 paginas*/
    __TAREA_2_CR3           = 0x00900000;
    __TAREA_3_CR3           = 0x01000000;
    __TAREA_4_CR3           = 0x01100000;

    __KERNEL_CR3            = 0x00010000;
    

    __INIT_16_PHI           = 0xFFFF0000;
    /*__INIT_32_PHI           = 0xFFFF0100;*/
    __SYS_TABLES_PHI        = 0xFFFF0300;
    __FUNCTIONS_ROM_PHI     = 0xFFFF0400;

    __UTILS_PHI             = 0xFFFF5000;
    __RESET_PHI             = 0xFFFFFFF0;

    /*****************VMA*******************/
    __SYS_TABLES_C_VMA      = 0x00000000;

    __PAGTABLES_VMA         = 0x00010000;
    __VIDEO_RAM_VMA         = 0x00010000;
    __INTERRUPTS_VMA        = 0x00100000;  
    __DATOS_VMA             = 0x01200000;
    __DATOS_USER_VMA        = 0x01201000;
    __MMX_DATA_VMA          = 0x01202000;
    __DIGITOS_VMA           = 0x01210000;
    __KERNEL_32_VMA         = 0x01220000;

    __CONTEXT_PAGE_TAREA_1_VMA = 0x01300000;
    __TAREA_1_TEXT          = 0x01310000;
    __TAREA_1_BSS           = 0x01320000;
    __TAREA_1_DATA          = 0x01330000;
    __TAREA_1_RODATA        = 0x01340000;
    
    __CONTEXT_PAGE_TAREA_2_VMA = 0x01400000;
    __TAREA_2_TEXT          = 0x01410000;
    __TAREA_2_BSS           = 0x01420000;
    __TAREA_2_DATA          = 0x01430000;
    __TAREA_2_RODATA        = 0x01440000;

    __CONTEXT_PAGE_TAREA_3_VMA = 0x01500000;
    __TAREA_3_TEXT          = 0x01510000;
    __TAREA_3_BSS           = 0x01520000;
    __TAREA_3_DATA          = 0x01530000;
    __TAREA_3_RODATA        = 0x01540000;

    __CONTEXT_PAGE_TAREA_4_VMA = 0x01600000;
    __TAREA_4_TEXT          = 0x01610000;
    __TAREA_4_BSS           = 0x01620000;
    __TAREA_4_DATA          = 0x01630000;
    __TAREA_4_RODATA        = 0x01640000;


    __PILA_KERNEL           = 0x1FFF8000;
    __TAREA_1_PILA          = 0x0078F000;
    __TAREA_2_PILA          = 0x00790000;
    __TAREA_3_PILA          = 0x00791000;
    __TAREA_4_PILA          = 0x00792000;

    __TAREA_1_PILA_KERNEL_VMA      = 0x2FFF4000;
    __TAREA_2_PILA_KERNEL_VMA      = 0x2FFF5000;
    __TAREA_3_PILA_KERNEL_VMA      = 0x2FFF6000;
    __TAREA_4_PILA_KERNEL_VMA      = 0x2FFF7000;

    __INIT_16_VMA           = 0xFFFF0000;
    __SYS_TABLES_VMA        = 0xFFFF0600;
    __UTILS_VMA             = 0xFFFF5000;
    __RESET_VMA             = 0xFFFFFFF0;

    /*****************LMA*******************/   /* La mayoria de los LMA los calculo en runtime */
    __INIT_16_LMA           = 0XFFFF0000;
    __SYS_TABLES_LMA        = 0xFFFF0600;
    __SYS_TABLES_C_LMA      = 0xFFFF4000;
    __TAREA_1_TEXT_LMA      = 0xFFFF7000;
    __RESET_LMA             = 0xFFFFFFF0;
 

MEMORY
{
    ram (!X) : ORIGIN = 0x00000000, LENGTH = 0xFFFF0000 /* Todo el bloque de 4Gb menos los ultimos 64K que los dejo en la rom */
    rom (rx) : ORIGIN = 0xFFFF0000, LENGTH = 0x0000FFFF /* Los ultimos 64k del mapa de 4Gb */
}

SECTIONS
{   
    .tablas_gdt_idt __SYS_TABLES_C_VMA :        /*0x000000*/
        AT ( __SYS_TABLES_C_LMA )
        {
            *(.systable_c);
            *(.interrupt_table_c);
            
        } > ram

    __INTERRUPTS_LMA = LOADADDR(.tablas_gdt_idt) + SIZEOF(.tablas_gdt_idt);

    .interrupts __INTERRUPTS_VMA :              /*0x00100000*/
        AT ( __INTERRUPTS_LMA )
        {
            *(.interrupt_code);
            "*int_handlers*"(.rodata);
        } > ram
    __codigo_interrupts_size = SIZEOF(.interrupts);

    __KERNEL_32_LMA = LOADADDR(.interrupts) + SIZEOF(.interrupts);

    .codigo_kernel32 __KERNEL_32_VMA :          /*0x01220000*/
        AT ( __KERNEL_32_LMA )
        {
            *(.kernel32);
            *(.functions_ram);
        } > ram
    __codigo_kernel32_size = SIZEOF(.codigo_kernel32);

    

    /* Codigo de la tarea 1 */
    .tarea_1_text __TAREA_1_TEXT :          /*0x01310000*/
        AT ( __TAREA_1_TEXT_LMA )
        {
            "*tarea_1*"(.text);
        } > ram
    __tarea_1_text_size = SIZEOF(.tarea_1_text);

    __TAREA_1_BSS_LMA = LOADADDR(.tarea_1_text) + SIZEOF(.tarea_1_text);

    .tarea_1_bss __TAREA_1_BSS :            /*0x01320000*/
        AT ( __TAREA_1_BSS_LMA )
        {
            "*tarea_1*"(.t1_bss);
            "*tarea_1*"(.bss);
        } > ram
    __tarea_1_bss_size = SIZEOF(.tarea_1_bss);

    __TAREA_1_DATA_LMA = LOADADDR(.tarea_1_bss) + SIZEOF(.tarea_1_bss);

    .tarea_1_data __TAREA_1_DATA :            /*0x01330000*/
        AT ( __TAREA_1_DATA_LMA )
        {
            "*tarea_1*"(.data);
        } > ram
    __tarea_1_data_size = SIZEOF(.tarea_1_data);

    __TAREA_1_RODATA_LMA = LOADADDR(.tarea_1_data) + SIZEOF(.tarea_1_data);

    .tarea_1_rodata __TAREA_1_RODATA :      /*0x01340000*/
        AT ( __TAREA_1_RODATA_LMA )
        {
            "*tarea_1*"(.rodata);
        } > ram
    __tarea_1_rodata_size = SIZEOF(.tarea_1_rodata);

    /* Codigo de la tarea 2 */
    __TAREA_2_TEXT_LMA = LOADADDR(.tarea_1_rodata) + SIZEOF(.tarea_1_rodata);

    .tarea_2_text __TAREA_2_TEXT :          /*0x01410000*/
        AT ( __TAREA_2_TEXT_LMA )
        {
            "*tarea_2*"(.text);
        } > ram
    __tarea_2_text_size = SIZEOF(.tarea_2_text);

    __TAREA_2_BSS_LMA = LOADADDR(.tarea_2_text) + SIZEOF(.tarea_2_text);

    .tarea_2_bss __TAREA_2_BSS :            /*0x01420000*/
        AT ( __TAREA_2_BSS_LMA )
        {
            "*tarea_2*"(.t2_bss);
        } > ram
    __tarea_2_bss_size = SIZEOF(.tarea_2_bss);

    __TAREA_2_DATA_LMA = LOADADDR(.tarea_2_bss) + SIZEOF(.tarea_2_bss);

    .tarea_2_data __TAREA_2_DATA :            /*0x01430000*/
        AT ( __TAREA_2_DATA_LMA )
        {
            "*tarea_2*"(.data);
        } > ram
    __tarea_2_data_size = SIZEOF(.tarea_2_data);

    __TAREA_2_RODATA_LMA = LOADADDR(.tarea_2_data) + SIZEOF(.tarea_2_data);

    .tarea_2_rodata __TAREA_2_RODATA :      /*0x01440000*/
        AT ( __TAREA_2_RODATA_LMA )
        {
            "*tarea_2*"(.rodata);
        } > ram
    __tarea_2_rodata_size = SIZEOF(.tarea_2_rodata);

    /* Codigo de la tarea 3 */
    __TAREA_3_TEXT_LMA = LOADADDR(.tarea_2_rodata) + SIZEOF(.tarea_2_rodata);

    .tarea_3_text __TAREA_3_TEXT :          /*0x01510000*/
        AT ( __TAREA_3_TEXT_LMA )
        {
            "*tarea_3*"(.text);
        } > ram
    __tarea_3_text_size = SIZEOF(.tarea_3_text);

    __TAREA_3_BSS_LMA = LOADADDR(.tarea_3_text) + SIZEOF(.tarea_3_text);

    .tarea_3_bss __TAREA_3_BSS :            /*0x01520000*/
        AT ( __TAREA_3_BSS_LMA )
        {
            "*tarea_3*"(.t3_bss);
        } > ram
    __tarea_3_bss_size = SIZEOF(.tarea_3_bss);

    __TAREA_3_DATA_LMA = LOADADDR(.tarea_3_bss) + SIZEOF(.tarea_3_bss);

    .tarea_3_data __TAREA_3_DATA :            /*0x01530000*/
        AT ( __TAREA_3_DATA_LMA )
        {
            "*tarea_3*"(.data);
        } > ram
    __tarea_3_data_size = SIZEOF(.tarea_3_data);

    __TAREA_3_RODATA_LMA = LOADADDR(.tarea_3_data) + SIZEOF(.tarea_3_data);

    .tarea_3_rodata __TAREA_3_RODATA :      /*0x01540000*/
        AT ( __TAREA_3_RODATA_LMA )
        {
            "*tarea_3*"(.rodata);
        } > ram
    __tarea_3_rodata_size = SIZEOF(.tarea_3_rodata);

    /* Codigo de la tarea 4 */
    __TAREA_4_TEXT_LMA = LOADADDR(.tarea_3_rodata) + SIZEOF(.tarea_3_rodata);
    .tarea_4_text __TAREA_4_TEXT :          /*0x01610000*/
        AT ( __TAREA_4_TEXT_LMA )
        {
            "*tarea_4*"(.text);
        } > ram
    __tarea_4_text_size = SIZEOF(.tarea_4_text);

     __TAREA_4_BSS_LMA = LOADADDR(.tarea_4_text) + SIZEOF(.tarea_4_text);

    .tarea_4_bss __TAREA_4_BSS :            /*0x01620000*/
        AT ( __TAREA_4_BSS_LMA )
        {
            "*tarea_4*"(.t4_bss);
        } > ram
    __tarea_4_bss_size = SIZEOF(.tarea_4_bss);

    __TAREA_4_DATA_LMA = LOADADDR(.tarea_4_bss) + SIZEOF(.tarea_4_bss);

    .tarea_4_data __TAREA_4_DATA :            /*0x01630000*/
        AT ( __TAREA_4_DATA_LMA )
        {
            "*tarea_4*"(.data);
        } > ram
    __tarea_4_data_size = SIZEOF(.tarea_4_data);

    __TAREA_4_RODATA_LMA = LOADADDR(.tarea_4_data) + SIZEOF(.tarea_4_data);

    .tarea_4_rodata __TAREA_4_RODATA :      /*0x01640000*/
        AT ( __TAREA_4_RODATA_LMA )
        {
            "*tarea_4*"(.rodata);
        } > ram
    __tarea_4_rodata_size = SIZEOF(.tarea_4_rodata);

   
    .codigo_init16 __INIT_16_VMA :
        AT ( __INIT_16_LMA )                /*0xFFFF0000*/
        {
            *(.ROM_init);
            *(.gateA20_code);
            *(.note*);
        } > rom
    
    __INIT_32_LMA = LOADADDR(.codigo_init16) + SIZEOF(.codigo_init16);
    __INIT_32_VMA = __INIT_32_LMA;
    
    .codigo_init32 __INIT_32_VMA :
        AT ( __INIT_32_LMA )                /*0xffff013c*/
        {
            *(.start32);
        } > rom
 
    .data __SYS_TABLES_VMA :                /*0xFFFF0500*/
        AT ( __SYS_TABLES_LMA ) 
        {
            *(.systables_rom);
        } > rom


    __FUNCTIONS_ROM_LMA = LOADADDR(.data) + SIZEOF(.data);
    __FUNCTIONS_ROM_VMA = __FUNCTIONS_ROM_LMA;

    .functions_rom __FUNCTIONS_ROM_VMA :
        AT ( __FUNCTIONS_ROM_LMA )
        {
            *(.functions_rom*);
        } > rom
    
    __DATOS_LMA = LOADADDR(.functions_rom) + SIZEOF(.functions_rom);

    .datos __DATOS_VMA :            /*0x01200000*/
        AT ( __DATOS_LMA )          /*0xffff0e84*/
        {
            *(.datos);
            *(.bss);
            *(.kernel_bss)
            *(.int_bss);
        } > ram

    __DATOS_USER_LMA = LOADADDR(.datos) + SIZEOF(.datos);

    .datos_user __DATOS_USER_VMA :            /*0x01201000*/
        AT ( __DATOS_USER_LMA )          /*0xffff0e84*/
        {
            *(.datos_user);
        } > ram

    __MMX_DATA_LMA = LOADADDR(.datos_user) + SIZEOF(.datos_user);

    /*Todas las tareas comparten la misma direccion lineal pero se mapean en direcciones fisicas distintas*/
    .mmx_data __MMX_DATA_VMA :      /*0x01202000*/
        AT  ( __MMX_DATA_LMA ) 
        {
            *(.mmx_data);
        } > ram

    __UTILS_LMA = LOADADDR(.mmx_data) + SIZEOF(.mmx_data);
    __UTILS_VMA = __UTILS_LMA;

    .utils16 __UTILS_VMA :          /*0xFFFF26C1*/
        AT ( __UTILS_LMA )          /*0xFFFF26C1*/
        {
            *(.utils16);
        } > rom                     /*0xFFFF39D1  Finalizacion*/ 

    .codigo_reset __RESET_VMA :     /*0xFFFFFFF0*/
        AT ( __RESET_LMA )          /*0xFFFFFFF0*/
        {
            *(.resetVector);
        } > rom

    /* esta seccion es opcional por si no quiero usar el ALIGN 16 en el reset vector */
    .fill_rom :
        {
            FILL(0x90);
            . = ORIGIN(rom) + LENGTH(rom) - 1;
            BYTE(0x90);
        } > rom
    
    /*Escribo el ultimo byte de la rom en NOP ya que el linker no va hasta el final */
    .end_rom 0xFFFFFFFF :
        AT ( 0xFFFFFFFF )
        {
            BYTE(0x90);
        }

}