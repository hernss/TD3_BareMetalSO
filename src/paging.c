#include "../inc/prototipos.h"

extern dword __PAGTABLES_VMA;
extern dword __CONTEXT_PAGE_TAREA_1_VMA;
extern dword __TAREA_1_TEXT;
extern dword __TAREA_1_DATA;
extern dword __TAREA_1_BSS;
extern dword __TAREA_1_RODATA;
extern dword __CONTEXT_PAGE_TAREA_2_VMA;
extern dword __TAREA_2_TEXT;
extern dword __TAREA_2_DATA;
extern dword __TAREA_2_BSS;
extern dword __TAREA_2_RODATA;
extern dword __CONTEXT_PAGE_TAREA_3_VMA;
extern dword __TAREA_3_TEXT;
extern dword __TAREA_3_DATA;
extern dword __TAREA_3_BSS;
extern dword __TAREA_3_RODATA;
extern dword __CONTEXT_PAGE_TAREA_4_VMA;
extern dword __TAREA_4_TEXT;
extern dword __TAREA_4_DATA;
extern dword __TAREA_4_BSS;
extern dword __TAREA_4_RODATA;
extern dword __VIDEO_RAM_VMA;
extern dword __INTERRUPTS_VMA;  
extern dword __DATOS_VMA;
extern dword __DATOS_USER_VMA;
extern dword __MMX_DATA_VMA;
extern dword __DIGITOS_VMA;
extern dword __KERNEL_32_VMA;
extern dword __PILA_KERNEL;
extern dword __TAREA_1_PILA;
extern dword __TAREA_2_PILA;
extern dword __TAREA_3_PILA;
extern dword __TAREA_4_PILA;
extern dword __TAREA_1_PILA_KERNEL_VMA;
extern dword __TAREA_2_PILA_KERNEL_VMA;
extern dword __TAREA_3_PILA_KERNEL_VMA;
extern dword __TAREA_4_PILA_KERNEL_VMA;

extern dword __PAGTABLES_PHI;
extern dword __CONTEXT_PAGE_TAREA_1_PHI;
extern dword __TAREA_1_TEXT_PHI;
extern dword __TAREA_1_DATA_PHI;
extern dword __TAREA_1_BSS_PHI;
extern dword __TAREA_1_RODATA_PHI;
extern dword __CONTEXT_PAGE_TAREA_2_PHI;
extern dword __TAREA_2_TEXT_PHI;
extern dword __TAREA_2_DATA_PHI;
extern dword __TAREA_2_BSS_PHI;
extern dword __TAREA_2_RODATA_PHI;
extern dword __CONTEXT_PAGE_TAREA_3_PHI;
extern dword __TAREA_3_TEXT_PHI;
extern dword __TAREA_3_DATA_PHI;
extern dword __TAREA_3_BSS_PHI;
extern dword __TAREA_3_RODATA_PHI;
extern dword __CONTEXT_PAGE_TAREA_4_PHI;
extern dword __TAREA_4_TEXT_PHI;
extern dword __TAREA_4_DATA_PHI;
extern dword __TAREA_4_BSS_PHI;
extern dword __TAREA_4_RODATA_PHI;
extern dword __VIDEO_RAM_PHI;
extern dword __INTERRUPTS_PHI;  
extern dword __DATOS_PHI;
extern dword __DATOS_USER_PHI;
extern dword __MMX_DATA_T1_PHI;
extern dword __MMX_DATA_T2_PHI;
extern dword __MMX_DATA_T3_PHI;
extern dword __MMX_DATA_T4_PHI;
extern dword __DIGITOS_PHI;
extern dword __KERNEL_32_PHI;
extern dword __PILA_KERNEL_PHI;
extern dword __TAREA_1_PILA_PHI;
extern dword __TAREA_2_PILA_PHI;
extern dword __TAREA_3_PILA_PHI;
extern dword __TAREA_4_PILA_PHI;
extern dword __TAREA_1_PILA_KERNEL_PHI;
extern dword __TAREA_2_PILA_KERNEL_PHI;
extern dword __TAREA_3_PILA_KERNEL_PHI;
extern dword __TAREA_4_PILA_KERNEL_PHI;
extern dword __TAREA_1_CR3;
extern dword __TAREA_2_CR3;
extern dword __TAREA_3_CR3;
extern dword __TAREA_4_CR3;

extern dword __KERNEL_CR3;

extern dword __RAMDOM_PAGE_START;

__attribute__(( section(".functions_rom"))) dword __create_paging (dword base_address, byte PCD, byte PWT){
    
    dword aux = base_address & 0xFFFFF000;  //Fuerzo a que los 12 bits LSB sean cero por las dudas..
    
    //Me aseguro que los flags sean 1
    if(PCD >= 1) PCD = 1;
    if(PWT >= 1) {PCD = 0; PWT = 1;}    //Si el Write Through esta activado tengo que tener habilitada la cache

    aux |= PCD << 4;
    aux |= PWT << 3;

    return aux;
}

__attribute__(( section(".functions_rom"))) dword __create_page_table (dword directory_base_address, word offset, dword page_table_base_address, byte PCD, byte PWT, byte USER_SUPERVISOR, byte READ_WRITE){

    dword* aux = (dword*)(directory_base_address & 0xFFFFF000);  //Fuerzo a que los 12 bits LSB sean cero por las dudas..
        
    aux += offset;                   //Desplazo 2 el offset para multiplicarlo por 4 ya que en la memoria me muevo de a 4 bytes y el offset es de 0 a 1023

    page_table_base_address = page_table_base_address & 0xFFFFF000;

    //Me aseguro que los flags sean 1
    if(PCD >= 1) PCD = 1;
    if(PWT >= 1) {PCD = 0; PWT = 1;}    //Si el Write Through esta activado tengo que tener habilitada la cache
    
    byte P = 1;                         //Bit presente, al menos por ahora va a arrancar en 1..


    if(USER_SUPERVISOR >= 1) USER_SUPERVISOR = 1;
    if(READ_WRITE >= 1) READ_WRITE = 1;  //Chequeo por las dudas no tener un valor distinto de 1 en los flags

    page_table_base_address |= PCD << 4;    
    page_table_base_address |= PWT << 3;
    page_table_base_address |= USER_SUPERVISOR << 2;    
    page_table_base_address |= READ_WRITE << 1;
    page_table_base_address |= P;

    *aux = page_table_base_address;         //Cargo el descriptor de la tabla de paginas en la posicion del directorio de tablas de paginas apuntado por directory+offset
    return page_table_base_address;
}

__attribute__(( section(".functions_rom"))) void __create_page (dword page_table_base_address, word offset, dword page_base_address, byte PCD, byte PWT, byte GLOBAL, byte USER_SUPERVISOR, byte READ_WRITE){

    dword* aux = (dword*)(page_table_base_address & 0xFFFFF000);  //Fuerzo a que los 12 bits LSB sean cero por las dudas..
    
    aux += offset;                   //Desplazo 2 el offset para multiplicarlo por 4 ya que en la memoria me muevo de a 4 bytes y el offset es de 0 a 1023

    page_base_address = page_base_address & 0xFFFFF000; //Fuero la limpieza de los 12bits LSB

    //Me aseguro que los flags sean 1
    if(PCD >= 1) PCD = 1;
    if(PWT >= 1) {PCD = 0; PWT = 1;}    //Si el Write Through esta activado tengo que tener habilitada la cache

    byte PS = 0;                        //Dejo esto por las dudas que quiera hacer una pag de 4MB
    byte P = 1;                         //Bit presente, al menos por ahora va a arrancar en 1..

    if(GLOBAL >= 1) GLOBAL = 1;
    if(USER_SUPERVISOR >= 1) USER_SUPERVISOR = 1;
    if(READ_WRITE >= 1) READ_WRITE = 1;  //Chequeo por las dudas no tener un valor distinto de 1 en los flags



    page_base_address |= GLOBAL << 8;    //Cargo los flags a la direccion de la tabla de paginas
    page_base_address |= PS << 7;
    //Dirty arranca en 0
    //Accesed arranca en 0
    page_base_address |= PCD << 4;    
    page_base_address |= PWT << 3;
    page_base_address |= USER_SUPERVISOR << 2;    
    page_base_address |= READ_WRITE << 1;
    page_base_address |= P;

    *aux = page_base_address;         //Cargo el descriptor de la pagina en la posicion de la tabla de paginas apuntado por page_table+offset
    return;
}

__attribute__(( section(".functions_rom"))) void __cargar_paginas_kernel(dword cr3){
    //Cargo las TP de Kernel
    dword systable_tp = __create_page_table(cr3, 0, cr3 + 0x1000, 0, 0, US_USER, RW_WRITE);                                             //Tabla de pagina  0x00000000 - 0x003FFFFF (lineales)
                                                                                                                                        //Tablas de sistema             0x00000000
                                                                                                                                        //Tablas de Paginacion          0x00010000
                                                                                                                                        //Video                         0x00010000
                                                                                                                                        //ISR                           0x00100000
    dword datos_digitos_kernel_tp = __create_page_table(cr3, EXTRACT_DTP_OFFSET(&__DATOS_VMA), cr3 + 0x2000, 0, 0, US_USER, RW_WRITE);  //Tabla de pagina  0x01000000 - 0x013FFFFF (lineales)
                                                                                                                                        //Datos                         0x01200000
                                                                                                                                        //Digitos                       0x01210000
                                                                                                                                        //Kernel                        0x01220000
    dword rom_tp = __create_page_table(cr3, EXTRACT_DTP_OFFSET(ROM_START), cr3 + 0x3000, 0, 0, US_USER, RW_WRITE);                      //Tabla de pagina  0xFFC00000 - 0xFFFFFFFF (lineales)
                                                                                                                                        //ROM                           0xFFFF0000

    //PAGINAS DENTRO DE LA TP systable_tp                                                                                                                               
    __create_page(systable_tp,0,0,0,0,1,US_SUP,RW_WRITE);                                                                               //PAGE      0x00000000-0x00000FFF(lin)      0x00000000-0x00000FFF(fis)
    __create_page(systable_tp,EXTRACT_TP_OFFSET(&__VIDEO_RAM_VMA),(dword)&__VIDEO_RAM_PHI,0,1,1,US_SUP,RW_WRITE);       //PWT=1         //PAGE      0x00010000-0x00010FFF(lin)      0x000B8000-0x000B8FFF(fis)
    __create_page(systable_tp,EXTRACT_TP_OFFSET(&__INTERRUPTS_VMA),(dword)&__INTERRUPTS_PHI,0,0,1,US_SUP,RW_READ);                      //PAGE      0x00100000-0x00100FFF(lin)      0x00100000-0x00100FFF(fis)
    __create_page(systable_tp,EXTRACT_TP_OFFSET(DPT_LINEAL),cr3,0,0,0,US_SUP,RW_WRITE);                                                 //PAGE      0x00009000-0x00009FFF(lin)             CR3-CR3+0xFFF


    //PAGINAS dentro de la tp datos_digitos_kernel_tp
    __create_page(datos_digitos_kernel_tp,EXTRACT_TP_OFFSET(&__DATOS_VMA),(dword)&__DATOS_PHI,0,0,1,US_SUP,RW_WRITE);                   //PAGE      0x01200000-0x01200FFF(lin)      0x00200000-0x00200FFF(fis)
    __create_page(datos_digitos_kernel_tp,EXTRACT_TP_OFFSET(&__DATOS_USER_VMA),(dword)&__DATOS_USER_PHI,0,0,1,US_USER,RW_WRITE);        //PAGE      0x01201000-0x01201FFF(lin)      0x00201000-0x00201FFF(fis)
  //__create_page(datos_digitos_kernel_tp,EXTRACT_TP_OFFSET(&__MMX_DATA_VMA),(dword)&__MMX_DATA_PHI,0,0,1,US_SUP,RW_WRITE);             //PAGE      0x01202000-0x01202FFF(lin)      0x00202000-0x00202FFF(fis)
    __create_page(datos_digitos_kernel_tp,EXTRACT_TP_OFFSET(&__DIGITOS_VMA),(dword)&__DIGITOS_PHI,0,0,1,US_SUP,RW_WRITE);               //PAGE      0x01210000-0x01210FFF(lin)      0x00210000-0x00210FFF(fis)
    __create_page(datos_digitos_kernel_tp,EXTRACT_TP_OFFSET(&__KERNEL_32_VMA),(dword)&__KERNEL_32_PHI,0,0,1,US_SUP,RW_READ);            //PAGE      0x01220000-0x01220FFF(lin)      0x00220000-0x00220FFF(fis)
    __create_page(datos_digitos_kernel_tp,EXTRACT_TP_OFFSET(&__KERNEL_32_VMA)+1,(dword)&__KERNEL_32_PHI+0x1000,0,0,1,US_SUP,RW_READ);   //PAGE      0x01221000-0x01221FFF(lin)      0x00221000-0x00221FFF(fis)

    //PAGINAS dentro de la rom
    //Creo las 16 paginas que necesito para mapear la ROM
    for(byte i=0; i<ROM_PAGE_COUNT; i++)
        __create_page(rom_tp,EXTRACT_TP_OFFSET(ROM_START+(i*0x1000)),ROM_START+(i*0x1000),0,0,0,US_SUP,RW_READ);                        //PAGE      0xFFFF0000-0xFFFFFFFF(lin)      0xFFFF0000-0xFFFFFFFF(fis)
}

__attribute__(( section(".functions_rom"))) dword inicializar_paginacion(){

    //Primero voy a cargar todo lo que es comun a todas las tareas


    //Creo la DTP de KERNEL con la que voy a inicializar la paginacion. Una vez que el scheduler este en marcha nunca mas vuelvo a este CR3
    dword cr3 =__create_paging((dword)&__KERNEL_CR3, 0, 0);
    dword tarea1_cr3 =__create_paging((dword)&__TAREA_1_CR3, 0, 0); 
    dword tarea2_cr3 =__create_paging((dword)&__TAREA_2_CR3, 0, 0); 
    dword tarea3_cr3 =__create_paging((dword)&__TAREA_3_CR3, 0, 0); 
    dword tarea4_cr3 =__create_paging((dword)&__TAREA_4_CR3, 0, 0); 
    
    __cargar_paginas_kernel(cr3);
    __cargar_paginas_kernel(tarea1_cr3);
    __cargar_paginas_kernel(tarea2_cr3);
    __cargar_paginas_kernel(tarea3_cr3);
    __cargar_paginas_kernel(tarea4_cr3);

    //Cargo las paginas de los contextos en el primer CR3 para poder inicializar los contextos de ejecucion
    //Una vez que hago el primer cambio de contexto nunca mas vuelvo a este CR3
    __create_page(cr3 + 0x2000,EXTRACT_TP_OFFSET(&__CONTEXT_PAGE_TAREA_1_VMA),(dword)&__CONTEXT_PAGE_TAREA_1_PHI,0,0,0,US_SUP,RW_WRITE);//PAGE      0x01300000-0x01300FFF(lin)      0x01300000-0x01300FFF(fis)

    dword context_tp = __create_page_table(cr3, EXTRACT_DTP_OFFSET(&__CONTEXT_PAGE_TAREA_2_VMA),cr3+0x5000,0,0,US_SUP,RW_WRITE);        //Tabla de pagina  0x01400000 - 0x017FFFFF (lineales)
    __create_page(context_tp,EXTRACT_TP_OFFSET(&__CONTEXT_PAGE_TAREA_2_VMA),(dword)&__CONTEXT_PAGE_TAREA_2_PHI,0,0,0,US_SUP,RW_WRITE);  //PAGE      0x01400000-0x01400FFF(lin)      0x01400000-0x01400FFF(fis)
    __create_page(context_tp,EXTRACT_TP_OFFSET(&__CONTEXT_PAGE_TAREA_3_VMA),(dword)&__CONTEXT_PAGE_TAREA_3_PHI,0,0,0,US_SUP,RW_WRITE);  //PAGE      0x01500000-0x01500FFF(lin)      0x01500000-0x01500FFF(fis)
    __create_page(context_tp,EXTRACT_TP_OFFSET(&__CONTEXT_PAGE_TAREA_4_VMA),(dword)&__CONTEXT_PAGE_TAREA_4_PHI,0,0,0,US_SUP,RW_WRITE);  //PAGE      0x01600000-0x01600FFF(lin)      0x01600000-0x01600FFF(fis)


    //Cargo la pagina de la pila de kernel
    dword pila_tp = __create_page_table(cr3,EXTRACT_DTP_OFFSET(&__PILA_KERNEL),cr3+0x4000,0,0,US_USER,RW_WRITE);                        //Tabla de pagina  0x1FFB0000 - 0x1FFFFFFF (lineales)
                                                                                                                                        //PILA KERNEL                   0x1FFF8000
    __create_page(pila_tp,EXTRACT_TP_OFFSET(&__PILA_KERNEL),(dword)&__PILA_KERNEL_PHI,0,0,1,US_SUP,RW_WRITE);                           //PAGE      0x1FFF8000-0x1FFF8FFF(lin)      0x1FFF8000-0x1FFF8FFF(fis)

    //Cargo la pagina de la pila de tarea1
    pila_tp = __create_page_table(tarea1_cr3,EXTRACT_DTP_OFFSET(&__TAREA_1_PILA),tarea1_cr3+0x4000,0,0,US_USER,RW_WRITE);               //Tabla de pagina  0x00400000 - 0x007FFFFF (lineales)
                                                                                                                                        //PILA TAREA1                   0x0078F000
    __create_page(pila_tp,EXTRACT_TP_OFFSET(&__TAREA_1_PILA),(dword)&__TAREA_1_PILA_PHI,0,0,1,US_USER,RW_WRITE);                        //PAGE      0x0078F000-0x0078FFFF(lin)      0x2FFFF000-0x2FFFFFFF(fis)

    //Cargo la pagina de la pila de tarea2
    pila_tp = __create_page_table(tarea2_cr3,EXTRACT_DTP_OFFSET(&__TAREA_2_PILA),tarea2_cr3+0x4000,0,0,US_USER,RW_WRITE);               //Tabla de pagina  0x00400000 - 0x007FFFFF (lineales)
                                                                                                                                        //PILA TAREA2                   0x00790000
    __create_page(pila_tp,EXTRACT_TP_OFFSET(&__TAREA_2_PILA),(dword)&__TAREA_2_PILA_PHI,0,0,1,US_USER,RW_WRITE);                        //PAGE      0x00790000-0x00790FFF(lin)      0x30000000-0x30000FFF(fis)

    //Cargo la pagina de la pila de tarea3
    pila_tp = __create_page_table(tarea3_cr3,EXTRACT_DTP_OFFSET(&__TAREA_3_PILA),tarea3_cr3+0x4000,0,0,US_USER,RW_WRITE);               //Tabla de pagina  0x00400000 - 0x007FFFFF (lineales)
                                                                                                                                        //PILA TAREA3                   0x00791000
    __create_page(pila_tp,EXTRACT_TP_OFFSET(&__TAREA_3_PILA),(dword)&__TAREA_3_PILA_PHI,0,0,1,US_USER,RW_WRITE);                        //PAGE      0x00791000-0x00791FFF(lin)      0x30001000-0x30001FFF(fis)

        //Cargo la pagina de la pila de tarea4
    pila_tp = __create_page_table(tarea4_cr3,EXTRACT_DTP_OFFSET(&__TAREA_4_PILA),tarea4_cr3+0x4000,0,0,US_USER,RW_WRITE);               //Tabla de pagina  0x00400000 - 0x007FFFFF (lineales)
                                                                                                                                        //PILA TAREA4                   0x00792000
    __create_page(pila_tp,EXTRACT_TP_OFFSET(&__TAREA_4_PILA),(dword)&__TAREA_4_PILA_PHI,0,0,1,US_SUP,RW_WRITE);                        //PAGE      0x00792000-0x00792FFF(lin)      0x30002000-0x30002FFF(fis)

    //Ahora cargo las pilas del kernel en cada cr3 de cada tarea
    //Cargo la pagina de la pila de kernel de la tarea1
    pila_tp = __create_page_table(tarea1_cr3,EXTRACT_DTP_OFFSET(&__TAREA_1_PILA_KERNEL_VMA),tarea1_cr3+0x6000,0,0,US_USER,RW_WRITE);    //Tabla de pagina  0x2FB00000 - 0x2FFFFFFF (lineales)                                                                                                                                    
    __create_page(pila_tp,EXTRACT_TP_OFFSET(&__TAREA_1_PILA_KERNEL_VMA),(dword)&__TAREA_1_PILA_KERNEL_PHI,0,0,1,US_SUP,RW_WRITE);       //PAGE      0x2FFF4000-0x2FFF4FFF(lin)      0x2FFF4000-0x2FFF4FFF(fis)

    //Cargo la pagina de la pila de kernel de la tarea2
    pila_tp = __create_page_table(tarea2_cr3,EXTRACT_DTP_OFFSET(&__TAREA_2_PILA_KERNEL_VMA),tarea2_cr3+0x6000,0,0,US_USER,RW_WRITE);    //Tabla de pagina  0x2FB00000 - 0x2FFFFFFF (lineales)
    __create_page(pila_tp,EXTRACT_TP_OFFSET(&__TAREA_2_PILA_KERNEL_VMA),(dword)&__TAREA_2_PILA_KERNEL_PHI,0,0,1,US_SUP,RW_WRITE);       //PAGE      0x2FFF5000-0x2FFF5FFF(lin)      0x2FFF5000-0x2FFF5FFF(fis)

    //Cargo la pagina de la pila de kernel de la tarea3
    pila_tp = __create_page_table(tarea3_cr3,EXTRACT_DTP_OFFSET(&__TAREA_3_PILA_KERNEL_VMA),tarea3_cr3+0x6000,0,0,US_USER,RW_WRITE);    //Tabla de pagina  0x2FB00000 - 0x2FFFFFFF (lineales)
    __create_page(pila_tp,EXTRACT_TP_OFFSET(&__TAREA_3_PILA_KERNEL_VMA),(dword)&__TAREA_3_PILA_KERNEL_PHI,0,0,1,US_SUP,RW_WRITE);       //PAGE      0x2FFF6000-0x2FFF6FFF(lin)      0x2FFF6000-0x2FFF6FFF(fis)

    //Cargo la pagina de la pila de kernel de la tarea4
    pila_tp = __create_page_table(tarea4_cr3,EXTRACT_DTP_OFFSET(&__TAREA_4_PILA_KERNEL_VMA),tarea4_cr3+0x6000,0,0,US_USER,RW_WRITE);    //Tabla de pagina  0x2FB00000 - 0x2FFFFFFF (lineales)
    __create_page(pila_tp,EXTRACT_TP_OFFSET(&__TAREA_4_PILA_KERNEL_VMA),(dword)&__TAREA_4_PILA_KERNEL_PHI,0,0,1,US_SUP,RW_WRITE);       //PAGE      0x2FFF7000-0x2FFF7FFF(lin)      0x2FFF7000-0x2FFF7FFF(fis)
    

    //Ahora que ya cargue las pilas, voy a cargar las paginas exclusivas de cada tarea (text, rodata, bss, data)
    //En TAREA1 ya tengo cargada TP ya que la comparte con el kernel, las otras tareas tengo que agregar la TP
    __create_page(tarea1_cr3+0x2000,EXTRACT_TP_OFFSET(&__MMX_DATA_VMA),(dword)&__MMX_DATA_T1_PHI,0,0,0,US_SUP,RW_WRITE);                        //PAGE      0x01202000-0x01202FFF(lin)      0x00301000-0x00301FFF(fis)
    __create_page(tarea1_cr3+0x2000,EXTRACT_TP_OFFSET(&__CONTEXT_PAGE_TAREA_1_VMA),(dword)&__CONTEXT_PAGE_TAREA_1_PHI,0,0,1,US_SUP,RW_WRITE);   //PAGE      0x01300000-0x01300FFF(lin)      0x00300000-0x00300FFF(fis)
    __create_page(tarea1_cr3+0x2000,EXTRACT_TP_OFFSET(&__TAREA_1_TEXT),(dword)&__TAREA_1_TEXT_PHI,0,0,0,US_USER,RW_READ);                       //PAGE      0x01310000-0x01310FFF(lin)      0x00310000-0x00310FFF(fis)
    __create_page(tarea1_cr3+0x2000,EXTRACT_TP_OFFSET(&__TAREA_1_BSS),(dword)&__TAREA_1_BSS_PHI,0,0,0,US_USER,RW_WRITE);                        //PAGE      0x01320000-0x01320FFF(lin)      0x00320000-0x00320FFF(fis)
    __create_page(tarea1_cr3+0x2000,EXTRACT_TP_OFFSET(&__TAREA_1_DATA),(dword)&__TAREA_1_DATA_PHI,0,0,0,US_USER,RW_WRITE);                      //PAGE      0x01330000-0x01330FFF(lin)      0x00330000-0x00330FFF(fis)
    __create_page(tarea1_cr3+0x2000,EXTRACT_TP_OFFSET(&__TAREA_1_RODATA),(dword)&__TAREA_1_RODATA_PHI,0,0,0,US_USER,RW_READ);                   //PAGE      0x01340000-0x01340FFF(lin)      0x00340000-0x00340FFF(fis)


    //TAREA 2
    dword tarea_tp = __create_page_table(tarea2_cr3,EXTRACT_DTP_OFFSET(&__TAREA_2_TEXT),tarea2_cr3+0x5000,0,0,US_USER,RW_WRITE);                //Tabla de pagina  0x01400000 - 0x017FFFFF (lineales)
    __create_page(tarea2_cr3+0x2000,EXTRACT_TP_OFFSET(&__MMX_DATA_VMA),(dword)&__MMX_DATA_T2_PHI,0,0,0,US_SUP,RW_WRITE);                        //PAGE      0x01202000-0x01202FFF(lin)      0x00401000-0x00401FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__CONTEXT_PAGE_TAREA_2_VMA),(dword)&__CONTEXT_PAGE_TAREA_2_PHI,0,0,1,US_SUP,RW_WRITE);            //PAGE      0x01400000-0x01400FFF(lin)      0x00400000-0x00400FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_2_TEXT),(dword)&__TAREA_2_TEXT_PHI,0,0,0,US_USER,RW_READ);                                //PAGE      0x01410000-0x01410FFF(lin)      0x00410000-0x00410FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_2_BSS),(dword)&__TAREA_2_BSS_PHI,0,0,0,US_USER,RW_WRITE);                                 //PAGE      0x01420000-0x01420FFF(lin)      0x00420000-0x00420FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_2_DATA),(dword)&__TAREA_2_DATA_PHI,0,0,0,US_USER,RW_WRITE);                               //PAGE      0x01430000-0x01430FFF(lin)      0x00430000-0x00430FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_2_RODATA),(dword)&__TAREA_2_RODATA_PHI,0,0,0,US_USER,RW_READ);                            //PAGE      0x01440000-0x01440FFF(lin)      0x00440000-0x00440FFF(fis)

    //TAREA 3
    tarea_tp = __create_page_table(tarea3_cr3,EXTRACT_DTP_OFFSET(&__TAREA_3_TEXT),tarea3_cr3+0x5000,0,0,US_USER,RW_WRITE);                      //Tabla de pagina  0x01400000 - 0x017FFFFF (lineales)
    __create_page(tarea3_cr3+0x2000,EXTRACT_TP_OFFSET(&__MMX_DATA_VMA),(dword)&__MMX_DATA_T3_PHI,0,0,0,US_SUP,RW_WRITE);                        //PAGE      0x01202000-0x01202FFF(lin)      0x00501000-0x00501FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__CONTEXT_PAGE_TAREA_3_VMA),(dword)&__CONTEXT_PAGE_TAREA_3_PHI,0,0,1,US_SUP,RW_WRITE);            //PAGE      0x01500000-0x01500FFF(lin)      0x00500000-0x00500FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_3_TEXT),(dword)&__TAREA_3_TEXT_PHI,0,0,0,US_USER,RW_READ);                                //PAGE      0x01510000-0x01510FFF(lin)      0x00510000-0x00510FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_3_BSS),(dword)&__TAREA_3_BSS_PHI,0,0,0,US_USER,RW_WRITE);                                 //PAGE      0x01520000-0x01520FFF(lin)      0x00520000-0x00520FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_3_DATA),(dword)&__TAREA_3_DATA_PHI,0,0,0,US_USER,RW_WRITE);                               //PAGE      0x01530000-0x01530FFF(lin)      0x00530000-0x00530FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_3_RODATA),(dword)&__TAREA_3_RODATA_PHI,0,0,0,US_USER,RW_READ);                            //PAGE      0x01540000-0x01540FFF(lin)      0x00540000-0x00540FFF(fis)

    //TAREA 4
    tarea_tp = __create_page_table(tarea4_cr3,EXTRACT_DTP_OFFSET(&__TAREA_4_TEXT),tarea4_cr3+0x5000,0,0,US_USER,RW_WRITE);                      //Tabla de pagina  0x01400000 - 0x017FFFFF (lineales)
    __create_page(tarea4_cr3+0x2000,EXTRACT_TP_OFFSET(&__MMX_DATA_VMA),(dword)&__MMX_DATA_T4_PHI,0,0,0,US_SUP,RW_WRITE);                        //PAGE      0x01202000-0x01202FFF(lin)      0x00601000-0x00601FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__CONTEXT_PAGE_TAREA_4_VMA),(dword)&__CONTEXT_PAGE_TAREA_4_PHI,0,0,1,US_SUP,RW_WRITE);            //PAGE      0x01600000-0x01600FFF(lin)      0x00600000-0x00600FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_4_TEXT),(dword)&__TAREA_4_TEXT_PHI,0,0,0,US_SUP,RW_READ);                                 //PAGE      0x01610000-0x01610FFF(lin)      0x00610000-0x00610FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_4_BSS),(dword)&__TAREA_4_BSS_PHI,0,0,0,US_SUP,RW_WRITE);                                  //PAGE      0x01620000-0x01620FFF(lin)      0x00620000-0x00620FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_4_DATA),(dword)&__TAREA_4_DATA_PHI,0,0,0,US_SUP,RW_WRITE);                                //PAGE      0x01630000-0x01630FFF(lin)      0x00630000-0x00630FFF(fis)
    __create_page(tarea_tp,EXTRACT_TP_OFFSET(&__TAREA_4_RODATA),(dword)&__TAREA_4_RODATA_PHI,0,0,0,US_SUP,RW_READ);                             //PAGE      0x01640000-0x01640FFF(lin)      0x00640000-0x00640FFF(fis)


    //Para la tarea 1 tengo que hacer el identity mapping de las TP y agregar una tp que apunte al comienzo de 0X0A000000
    __create_page_table(tarea1_cr3,EXTRACT_DTP_OFFSET(0x0A000000),tarea1_cr3+0x5000,0,0,US_SUP,RW_WRITE);                                       //Tabla de pagina  0x0A000000 - 0x0A3FFFFF (lineales)

    //Tengo que cargar los identity mapping de la DTP y de las TPs para poder verificar que la direccion a escribir sea de modo usuario
    //Ahora cargo el identity mapping de tarea1_cr3                                                                                                                
    dword tarea1_cr3_TP = __create_page_table(tarea1_cr3,EXTRACT_DTP_OFFSET(tarea1_cr3),tarea1_cr3+0x7000,0,0,US_USER,RW_WRITE);                //Tabla de pagina  0x00800000 - 0x00BFFFFF (lineales)
    __create_page(tarea1_cr3_TP,EXTRACT_TP_OFFSET(tarea1_cr3),tarea1_cr3,0,0,0,US_SUP,RW_READ);                                                //PAGE      0x00800000-0x00800FFF(lin)      0x00800000-0x00800FFF(lin)
    __create_page(tarea1_cr3_TP,EXTRACT_TP_OFFSET(tarea1_cr3+0x1000),tarea1_cr3+0x1000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00801000-0x00801FFF(lin)      0x00801000-0x00801FFF(lin)
    __create_page(tarea1_cr3_TP,EXTRACT_TP_OFFSET(tarea1_cr3+0x2000),tarea1_cr3+0x2000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00802000-0x00802FFF(lin)      0x00802000-0x00802FFF(lin)
    __create_page(tarea1_cr3_TP,EXTRACT_TP_OFFSET(tarea1_cr3+0x3000),tarea1_cr3+0x3000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00803000-0x00803FFF(lin)      0x00803000-0x00803FFF(lin)
    __create_page(tarea1_cr3_TP,EXTRACT_TP_OFFSET(tarea1_cr3+0x4000),tarea1_cr3+0x4000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00804000-0x00804FFF(lin)      0x00804000-0x00804FFF(lin)
    __create_page(tarea1_cr3_TP,EXTRACT_TP_OFFSET(tarea1_cr3+0x5000),tarea1_cr3+0x5000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00805000-0x00805FFF(lin)      0x00805000-0x00805FFF(lin)
    __create_page(tarea1_cr3_TP,EXTRACT_TP_OFFSET(tarea1_cr3+0x6000),tarea1_cr3+0x6000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00806000-0x00806FFF(lin)      0x00806000-0x00806FFF(lin)
    __create_page(tarea1_cr3_TP,EXTRACT_TP_OFFSET(tarea1_cr3+0x7000),tarea1_cr3+0x7000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00807000-0x00807FFF(lin)      0x00807000-0x00807FFF(lin)

    //Ahora cargo el identity mapping de tarea2_cr3   
    dword tarea2_cr3_TP = __create_page_table(tarea2_cr3,EXTRACT_DTP_OFFSET(tarea2_cr3),tarea2_cr3+0x7000,0,0,US_USER,RW_WRITE);                //Tabla de pagina  0x00800000 - 0x00BFFFFF (lineales)
    __create_page(tarea2_cr3_TP,EXTRACT_TP_OFFSET(tarea2_cr3),tarea2_cr3,0,0,0,US_SUP,RW_READ);                                                //PAGE      0x00900000-0x00900FFF(lin)      0x00900000-0x00900FFF(lin)
    __create_page(tarea2_cr3_TP,EXTRACT_TP_OFFSET(tarea2_cr3+0x1000),tarea2_cr3+0x1000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00901000-0x00901FFF(lin)      0x00901000-0x00901FFF(lin)
    __create_page(tarea2_cr3_TP,EXTRACT_TP_OFFSET(tarea2_cr3+0x2000),tarea2_cr3+0x2000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00902000-0x00902FFF(lin)      0x00902000-0x00902FFF(lin)
    __create_page(tarea2_cr3_TP,EXTRACT_TP_OFFSET(tarea2_cr3+0x3000),tarea2_cr3+0x3000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00903000-0x00903FFF(lin)      0x00903000-0x00903FFF(lin)
    __create_page(tarea2_cr3_TP,EXTRACT_TP_OFFSET(tarea2_cr3+0x4000),tarea2_cr3+0x4000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00904000-0x00904FFF(lin)      0x00904000-0x00904FFF(lin)
    __create_page(tarea2_cr3_TP,EXTRACT_TP_OFFSET(tarea2_cr3+0x5000),tarea2_cr3+0x5000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00905000-0x00905FFF(lin)      0x00905000-0x00905FFF(lin)
    __create_page(tarea2_cr3_TP,EXTRACT_TP_OFFSET(tarea2_cr3+0x6000),tarea2_cr3+0x6000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00906000-0x00906FFF(lin)      0x00906000-0x00906FFF(lin)
    __create_page(tarea2_cr3_TP,EXTRACT_TP_OFFSET(tarea2_cr3+0x7000),tarea2_cr3+0x7000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x00907000-0x00907FFF(lin)      0x00907000-0x00907FFF(lin)

    //Ahora cargo el identity mapping de tarea3_cr3   
    dword tarea3_cr3_TP = tarea3_cr3 + 0x2000;     //(Ya esta cargada esta TP)                                                                 //Tabla de pagina  0x01000000 - 0x013FFFFF (lineales)
    __create_page(tarea3_cr3_TP,EXTRACT_TP_OFFSET(tarea3_cr3),tarea3_cr3,0,0,0,US_SUP,RW_READ);                                                //PAGE      0x01000000-0x01000FFF(lin)      0x01000000-0x01000FFF(lin)
    __create_page(tarea3_cr3_TP,EXTRACT_TP_OFFSET(tarea3_cr3+0x1000),tarea3_cr3+0x1000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x01001000-0x01001FFF(lin)      0x01001000-0x01001FFF(lin)
    __create_page(tarea3_cr3_TP,EXTRACT_TP_OFFSET(tarea3_cr3+0x2000),tarea3_cr3+0x2000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x01002000-0x01002FFF(lin)      0x01002000-0x01002FFF(lin)
    __create_page(tarea3_cr3_TP,EXTRACT_TP_OFFSET(tarea3_cr3+0x3000),tarea3_cr3+0x3000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x01003000-0x01003FFF(lin)      0x01003000-0x01003FFF(lin)
    __create_page(tarea3_cr3_TP,EXTRACT_TP_OFFSET(tarea3_cr3+0x4000),tarea3_cr3+0x4000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x01004000-0x01004FFF(lin)      0x01004000-0x01004FFF(lin)
    __create_page(tarea3_cr3_TP,EXTRACT_TP_OFFSET(tarea3_cr3+0x5000),tarea3_cr3+0x5000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x01005000-0x01005FFF(lin)      0x01005000-0x01005FFF(lin)
    __create_page(tarea3_cr3_TP,EXTRACT_TP_OFFSET(tarea3_cr3+0x6000),tarea3_cr3+0x6000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x01006000-0x01006FFF(lin)      0x01006000-0x01006FFF(lin)
    __create_page(tarea3_cr3_TP,EXTRACT_TP_OFFSET(tarea3_cr3+0x7000),tarea3_cr3+0x7000,0,0,0,US_SUP,RW_READ);                                  //PAGE      0x01007000-0x01007FFF(lin)      0x01007000-0x01007FFF(lin)

    return cr3;

}

__attribute__(( section(".functions_rom"))) dword __get_new_page(){
    static dword last_page_address = 0;

    if(last_page_address == 0){
        //Si es la primera vez que entro le cargo el comienzo de la direccion de tablas de paginas y retorno ese valor
        last_page_address = (dword)&__PAGTABLES_VMA;
    }else{
        //si no es la primera vez, desplazo 4k y retorno ese valor, como es static me queda guardado para la proxima
        last_page_address += 0x1000;          
    }

    return last_page_address;
}

__attribute__(( section(".functions_rom"))) dword __get_new_random_page(){
    static dword last_page_address = 0;

    if(last_page_address == 0){
        //Si es la primera vez que entro le cargo el comienzo de la direccion de tablas de paginas y retorno ese valor
        last_page_address = (dword)&__RAMDOM_PAGE_START;
    }else{
        //si no es la primera vez, desplazo 4k y retorno ese valor, como es static me queda guardado para la proxima
        last_page_address += 0x1000;
    }  

    if (last_page_address == 0x0A400000){
        //Llegue al final de mis capacidades. sabiamos que esta patraña no iba a durar para siempre, voy a llamar a washington para decirles que ellos ganan
        char str[15] = "Out of memory.";
        printlog(str);
        __asm__ __volatile__ ("cli");
        __asm__ __volatile__ ("hlt");
    }

    return last_page_address;
}

__attribute__(( section(".functions_rom"))) byte test_DTP(dword cr3, word dtp_offset){
    dword* aux = (dword*)(cr3 & 0xFFFFF000);    //Fuerzo a que los 12 bits LSB sean cero por las dudas..
    
    aux += dtp_offset;                          //Desplazo 2 el offset para multiplicarlo por 4 ya que en la memoria me muevo de a 4 bytes y el offset es de 0 a 1023

    if(*aux){                                   //Si es distinto de 0 es porque tengo valor cargado
        return 1;
    }

    return 0;                                   //Si no hay datos devuelvo 0
}

__attribute__(( section(".functions_rom"))) byte test_TP(dword dte, word tp_offset){
    dword* aux = (dword*)(dte & 0xFFFFF000);    //Fuerzo a que los 12 bits LSB sean cero por las dudas..
    
    aux += tp_offset;                           //Desplazo 2 el offset para multiplicarlo por 4 ya que en la memoria me muevo de a 4 bytes y el offset es de 0 a 1023

    if(*aux){                                   //Si es distinto de 0 es porque tengo valor cargado
        return 1;                               //Entonces devuelvo 1
    }

    return 0;                                   //Si no hay datos devuelvo 0
}
