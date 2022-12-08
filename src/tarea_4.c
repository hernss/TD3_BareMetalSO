#include "../inc/prototipos.h"

__attribute__ ((aligned (8))) void Tarea_idle(){
    //Tarea que hace dormir al procesador
    //__asm__ __volatile__ ("xchg %bx, %bx");
    while(1){
        __asm__ __volatile__ ("hlt");   //Puedo llamar a hlt directamente porque Tarea 4 es de nivel 0
    }
    return;
}
