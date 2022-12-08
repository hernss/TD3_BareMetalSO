#include "../inc/prototipos.h"

extern dword __PILA_KERNEL;
extern dword __TAREA_1_PILA;
extern dword __TAREA_2_PILA;
extern dword __TAREA_3_PILA;
extern dword __TAREA_4_PILA;
extern dword __TAREA_1_PILA_KERNEL_VMA;
extern dword __TAREA_2_PILA_KERNEL_VMA;
extern dword __TAREA_3_PILA_KERNEL_VMA;
extern dword __TAREA_4_PILA_KERNEL_VMA;

extern dword __TAREA_1_CR3;
extern dword __TAREA_2_CR3;
extern dword __TAREA_3_CR3;
extern dword __TAREA_4_CR3;
extern dword __CONTEXT_PAGE_TAREA_1_VMA;
extern dword __CONTEXT_PAGE_TAREA_2_VMA;
extern dword __CONTEXT_PAGE_TAREA_3_VMA;
extern dword __CONTEXT_PAGE_TAREA_4_VMA;


//Funciones definidas en asembler (main.s)
extern void save_context(item_faketss*);
extern void load_context(item_faketss*, dword cr3);

#pragma pack(2)
__attribute__(( section(".kernel_bss"))) item_tarea list_tareas[CANTIDAD_TAREAS];

__attribute__(( section(".functions_ram"))) void inicializar_scheduler(){

    list_tareas[0].cs = CS_SEL_C_USER;
    list_tareas[0].ip = (dword)&Tarea_promedio;
    list_tareas[0].ss = DS_SEL_C_USER;
    list_tareas[0].stack_pointer = (dword)&__TAREA_1_PILA;
    list_tareas[0].status = stop;
    list_tareas[0].pending_running = 0;
    list_tareas[0].cr3 = (dword)&__TAREA_1_CR3;
    list_tareas[0].context = (item_faketss*)&__CONTEXT_PAGE_TAREA_1_VMA;

    list_tareas[0].context->cs = CS_SEL_C_USER;
    list_tareas[0].context->ds = DS_SEL_C_USER;
    list_tareas[0].context->es = DS_SEL_C_USER;
    list_tareas[0].context->fs = DS_SEL_C_USER;
    list_tareas[0].context->gs = DS_SEL_C_USER;
    list_tareas[0].context->ss3 = DS_SEL_C_USER;
    list_tareas[0].context->esp3 = (dword)&__TAREA_1_PILA;
    list_tareas[0].context->esp3 += 0xFF0;          
    list_tareas[0].context->ss0 = DS_SEL_C;
    list_tareas[0].context->esp0 = (dword)&__TAREA_1_PILA_KERNEL_VMA;
    list_tareas[0].context->esp0 += 0xFFF;        
    list_tareas[0].context->cr3 = (dword)&__TAREA_1_CR3;
    list_tareas[0].context->eip = (dword)&Tarea_promedio;
    list_tareas[0].context->eflags = 0x202 | 0x00040000;
    list_tareas[0].context->context_id = 0;
    list_tareas[0].context->flag_mmx = 0;
    




    list_tareas[1].cs = CS_SEL_C_USER;
    list_tareas[1].ip = (dword)&Tarea_suma1;
    list_tareas[1].ss = DS_SEL_C_USER;
    list_tareas[1].stack_pointer = (dword)&__TAREA_2_PILA;
    list_tareas[1].status = stop;
    list_tareas[1].pending_running = 0;
    list_tareas[1].cr3 = (dword)&__TAREA_2_CR3;
    list_tareas[1].context = (item_faketss*)&__CONTEXT_PAGE_TAREA_2_VMA;
    list_tareas[1].context->cs = CS_SEL_C_USER;
    list_tareas[1].context->ds = DS_SEL_C_USER;
    list_tareas[1].context->es = DS_SEL_C_USER;
    list_tareas[1].context->fs = DS_SEL_C_USER;
    list_tareas[1].context->gs = DS_SEL_C_USER;
    list_tareas[1].context->ss3 = DS_SEL_C_USER;
    list_tareas[1].context->esp3 = (dword)&__TAREA_2_PILA;
    list_tareas[1].context->esp3 += 0xFF0;          
    list_tareas[1].context->ss0 = DS_SEL_C;
    list_tareas[1].context->esp0 = (dword)&__TAREA_2_PILA_KERNEL_VMA;
    list_tareas[1].context->esp0 += 0xFFF;        
    list_tareas[1].context->cr3 = (dword)&__TAREA_2_CR3;
    list_tareas[1].context->eip = (dword)&Tarea_suma1;
    list_tareas[1].context->eflags = 0x202 | 0x00040000;
    list_tareas[1].context->context_id = 1;
    list_tareas[1].context->flag_mmx = 0;



    list_tareas[2].cs = CS_SEL_C_USER;
    list_tareas[2].ip = (dword)&Tarea_suma2;
    list_tareas[2].ss = DS_SEL_C_USER;
    list_tareas[2].stack_pointer = (dword)&__TAREA_3_PILA;
    list_tareas[2].status = stop;
    list_tareas[2].pending_running = 0;
    list_tareas[2].cr3 = (dword)&__TAREA_3_CR3;
    list_tareas[2].context = (item_faketss*)&__CONTEXT_PAGE_TAREA_3_VMA;
    list_tareas[2].context->cs = CS_SEL_C_USER;
    list_tareas[2].context->ds = DS_SEL_C_USER;
    list_tareas[2].context->es = DS_SEL_C_USER;
    list_tareas[2].context->fs = DS_SEL_C_USER;
    list_tareas[2].context->gs = DS_SEL_C_USER;
    list_tareas[2].context->ss3 = DS_SEL_C_USER;
    list_tareas[2].context->esp3 = (dword)&__TAREA_3_PILA;
    list_tareas[2].context->esp3 += 0xFF0;          
    list_tareas[2].context->ss0 = DS_SEL_C;
    list_tareas[2].context->esp0 = (dword)&__TAREA_3_PILA_KERNEL_VMA;
    list_tareas[2].context->esp0 += 0xFFF;        
    list_tareas[2].context->cr3 = (dword)&__TAREA_3_CR3;
    list_tareas[2].context->eip = (dword)&Tarea_suma2;
    list_tareas[2].context->eflags = 0x202 | 0x00040000;
    list_tareas[2].context->context_id = 2;
    list_tareas[2].context->flag_mmx = 0;



    list_tareas[3].cs = CS_SEL_C;
    list_tareas[3].ip = (dword)&Tarea_idle;
    list_tareas[3].ss = DS_SEL_C;
    list_tareas[3].stack_pointer = (dword)&__TAREA_4_PILA;
    list_tareas[3].status = stop;
    list_tareas[3].pending_running = 0;
    list_tareas[3].cr3 = (dword)&__TAREA_4_CR3;
    //Cargo el contexto por defecto de la tarea idle
    list_tareas[3].context = (item_faketss*)&__CONTEXT_PAGE_TAREA_4_VMA;
    list_tareas[3].context->cs = CS_SEL_C;
    list_tareas[3].context->ds = DS_SEL_C;
    list_tareas[3].context->es = DS_SEL_C;
    list_tareas[3].context->fs = DS_SEL_C;
    list_tareas[3].context->gs = DS_SEL_C;
    list_tareas[3].context->ss3 = DS_SEL_C;
    list_tareas[3].context->esp3 = (dword)&__TAREA_4_PILA;
    list_tareas[3].context->esp3 += 0xFF0;          
    list_tareas[3].context->ss0 = DS_SEL_C;
    list_tareas[3].context->esp0 = (dword)&__TAREA_4_PILA_KERNEL_VMA;
    list_tareas[3].context->esp0 += 0xFFF;        
    list_tareas[3].context->cr3 = (dword)&__TAREA_4_CR3;
    list_tareas[3].context->eip = (dword)&Tarea_idle;
    list_tareas[3].context->eflags = 0x202 | 0x00040000;
    list_tareas[3].context->context_id = 3;
    list_tareas[3].context->flag_mmx = 0;

    
}

__attribute__(( section(".kernel_bss"))) byte flag_no_pending;

__attribute__(( section(".functions_ram"))) void change_context(){

flag_no_pending = 1;
//Bucle por lista de tareas para buscar la tarea que esta corriendo..
//Si hay alguna tarea corriendo entonces le salvo el contexto
//Si la llamada a cambio de contexto se dio en la primera vez que corre el timer tick no va a haber ninguna tarea corriendo por ende nada que salvar
for(byte i=0;i<CANTIDAD_TAREAS;i++){
    //Busco la tarea que este corriendo.. si es que la hay...
    if(list_tareas[i].status == running){
        save_context(list_tareas[i].context);   //GFuardo el contexto en la direccion apuntada por el campo context
        list_tareas[i].status = stop;           //Le pongo el estado stop
        break;                                  //Ya encontre la tarea que estaba corriendo, no necesito controlar las otras
    }
}
//Busco la proxima tarea a ejecutar analizando los pending running
//Cuando encuentro la primera tareea que es distinta de 0, decremento y le cargo el contexto a esta tarea

for(byte i=0;i<CANTIDAD_TAREAS;i++){
    if(list_tareas[i].pending_running){
        list_tareas[i].pending_running--;

        list_tareas[i].status = running;        //Le pongo el estado running
        load_context(list_tareas[i].context, list_tareas[i].cr3);   //Cargo el contexto de la direccion apuntada por el campo context
        flag_no_pending = 0;
        break;                                  //Ya encontre la tarea que tengo que cargar, no necesito controlar las otras
    }
}

//Si no tengo ninguna tarea en pending running cambio de contexto a la tarea idle
if(flag_no_pending){
    //Quiere decir que no tengo nada pendiente para correr... entonces cargo la idle
    load_context(list_tareas[3].context, list_tareas[3].cr3);
    list_tareas[3].status = running;
}

return;
}