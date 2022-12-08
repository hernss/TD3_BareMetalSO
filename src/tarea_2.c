#include "../inc/prototipos.h"

__attribute__(( section(".datos_user"))) __attribute__ ((aligned (8))) qword suma_sw = 0;
__attribute__(( section(".datos_user"))) __attribute__ ((aligned (8))) qword suma = 0;        //Queda por compativilidad
__attribute__(( section(".t2_bss"))) __attribute__ ((aligned (8))) qword aux_t2;
__attribute__(( section(".t2_bss"))) __attribute__ ((aligned (8))) qword acc_t2;

__attribute__ ((aligned (8))) void Tarea_suma1(){
    
    while(1){
        __attribute__ ((aligned (8))) word item_count;

        __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_READ_COUNT));
        __asm__ __volatile__ ("int $0x80");         //Hago la syscall para leer la cantidad de datos que tengo en la tabla ya que es una variable que maneja el kernel
        __asm__ __volatile__ ("mov %%ax, %0": "=m"(item_count));

        item_count >>= 3;        //Divido table offset por 8 para obtener la cantidad de elementos que tiene la table

        if(item_count!=0){
            aux_t2=0;
            acc_t2=0;
            for(word i=0;i<item_count;i++){
                __asm__ __volatile__ ("mov %0, %%ebx": : "r"(&aux_t2));
                __asm__ __volatile__ ("mov %0, %%cx": : "r"(i));
                __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_READ));         //Leo la posicion i de la tabla de digitos y lo almaceno en aux_t2 para luego acumularla
                __asm__ __volatile__ ("int $0x80");                                 

                sum_sw(&acc_t2, &aux_t2);     //acc_t2 = acc_t2 + aux_t2 saturado en word

            }
            suma_sw = acc_t2;
            
        }
        __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_SHOW_ADD_SW));
        __asm__ __volatile__ ("int $0x80");         //Hago la syscall para forzar el cambio de contexto, cuando tenga prioridades este cambio se hace a nivel kernel
        __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_TASK_SWITCH));
        __asm__ __volatile__ ("int $0x80");         //Hago la syscall para forzar el cambio de contexto, cuando tenga prioridades este cambio se hace a nivel kernel
        __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_HLT));
        __asm__ __volatile__ ("int $0x80");         //Hago la syscall para irme a dormir
    }
    return;
}
