#include "../inc/prototipos.h"

extern qword promedio;
extern item_tarea list_tareas[CANTIDAD_TAREAS];
byte print_error(dword ERROR_CODE, dword cr2);

//__attribute__(( section(".mmx_data"))) byte mmx_context_data[CANTIDAD_TAREAS][512];
__attribute__(( section(".kernel_bss"))) word last_context_id;
__attribute__(( section(".kernel_bss"))) word actual_context_id;             //Variables globales de kernel
__attribute__(( section(".mmx_data"))) byte mmx_context_data[512];        //Cada tarea lo va a tener mapeado a una direccion fisica distinta

//Variables globales para que no me afecten la pila del PIT0.. el cambio de contexto romperia las bolas
__attribute__(( section(".int_bss"))) word ticks = 0;  
__attribute__(( section(".int_bss"))) byte first_time;
__attribute__(( section(".int_bss"))) byte flag_change_context;
__attribute__(( section(".int_bss"))) char global_char = 0;

__attribute__(( section(".interrupt_code"))) byte handler_NM(){
    __asm__ __volatile__ ("push %eax");
    __asm__ __volatile__ ("push %edx");

    __asm__ __volatile__ ("clts");       //Bajo el flag TS del CR0
    //Primero chequeo si el context id anterior es valido, o si el contexto anterior y el actual es el mismo,
    //en ese caso no tengo que guardar nada. Actualizo el last_context_id por si llega a ser la primera vez que entro
    if((last_context_id==0xFFFF)||(last_context_id == actual_context_id)){
        last_context_id = actual_context_id;
    }else{
        //Recupero el contexto de la tarea actual
        __asm__ __volatile__("fxrstor %0" : : "m" (mmx_context_data));
        last_context_id = actual_context_id;
        
    }
        //__asm__ __volatile__ ("xchg %bx, %bx");
    list_tareas[actual_context_id].context->flag_mmx = 1;
    __asm__ __volatile__ ("pop %edx");
    __asm__ __volatile__ ("pop %eax");
    __asm__ __volatile__ ("leave");
    __asm__ __volatile__ ("iret");
    return 1;
}

__attribute__(( section(".interrupt_code"))) byte handler_teclado(){
    __asm__ __volatile__ ("push %eax");
    __asm__ __volatile__ ("push %ebx");
    unsigned char keycode;
    keycode = inb(0x60);            //Leo la tecla presionada

    keycode &= 0xFF;                //Fuerzo a solo 8bits para no probocar un posible desbodamiento

    if(!(keycode & 0x80)){          //Capturo el MAKE y descarto el BREAK
        global_char = get_ascii(keycode);   //Pongo el caracter disponible en la variable global para que lo pueda capturar desde el loop

        print(global_char);                 //Imprimo el caracter en pantalla

        almacenar_buffer(global_char);      //Cargo la tecla en el buffer
        
        check_list();                       //Me fijo si tengo dato nuevo para agregar a la lista
        
        if(global_char == 'P'){
            char texto[] = "Tecla P presionada";

            __asm__ __volatile__ ("mov %0, %%ebx": : "r"(&texto));
            __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_PRINT));
            __asm__ __volatile__ ("int $0x80");         //Hago la syscall para imprimir en pantalla
        }

    }

    __asm__ __volatile__ ("sti");   
    outb(0x61, 0x20);               //EOI para que vuelva a interrumpir el teclado
    __asm__ __volatile__ ("pop %ebx");
    __asm__ __volatile__ ("pop %eax");
    __asm__ __volatile__ ("leave");
    __asm__ __volatile__ ("iret");
    return 1;
}




__attribute__(( section(".interrupt_code"))) byte handler_PIT0(){
    //Handler para el canal 0 del pit
    //Aca me llega una interrupcion cada 10ms

    __asm__ __volatile__ ("push %eax");
    __asm__ __volatile__ ("push %ebx");
    __asm__ __volatile__ ("push %ecx");
    __asm__ __volatile__ ("push %edx");
    __asm__ __volatile__ ("push %edi");
    __asm__ __volatile__ ("push %esi");



    first_time=1;

    for(byte i=0;i<CANTIDAD_TAREAS;i++){
        if(list_tareas[i].status != stop){
            first_time = 0;                     //Si entro aca es porque la tarea no esta en stop.. significa que no es la primera vez que entro al systick
        }
    }

    if(first_time){      
        flag_change_context=0;                   //Si es la primera vez que ejecuto cargo el contexto de la tarea idle y hago el cambio de contexto
        list_tareas[3].pending_running++;       //Le incremento el running a la tarea 4 que es la idle
        change_context();
        __asm__ __volatile__ ("sti");   
        outb(0x60, 0x20);                       //EOI para que vuelva a interrumpir el timer tick
        ticks++; 
        __asm__ __volatile__ ("pop %esi");
        __asm__ __volatile__ ("pop %edi");
        __asm__ __volatile__ ("pop %edx");
        __asm__ __volatile__ ("pop %ecx");
        __asm__ __volatile__ ("pop %ebx");
        __asm__ __volatile__ ("pop %eax");
        __asm__ __volatile__ ("leave");
        __asm__ __volatile__ ("iret");          
        //Cuando hago el iret como ya tengo el contexto cambiado retorno a la tarea 0
    }

    
    if(!(ticks%10)){
        // Aca entra cada 100 ms, llamo a tarea suma 1 (list_tareas[1])
        flag_change_context = 1;              //Subo el flag para que haga el cambio de contexto al final
        list_tareas[1].pending_running ++;    
    }

    if(!(ticks%20)){ 
        //Aca entro cada 200ms, llamo a tarea suma 2 (list_tareas[2])
        flag_change_context = 1;              //Subo el flag para que haga el cambio de contexto al final
        list_tareas[2].pending_running ++;    
    }

    if(!(ticks%50)){
        //mostrar_promedio(promedio);
        flag_change_context = 1;              //Subo el flag para que haga el cambio de contexto al final
        list_tareas[0].pending_running ++;    
    }

    //Variable global que me marca el ticks en el main loop
    if (ticks >= 100){
        ticks = 0;
        add_progress();     //Incremento la progress bar de la GUI
    }else {
        ticks++;    
    }

    if(flag_change_context){                
        flag_change_context = 0;            //Aca entro en dos situaciones. Alguna de las tareas tiene ejecucion pendiente o alguna tarea que estaba corriento termino su ejecucion y levanto el flag
                                            //En ese caso, se fuerza el cambio de contexto, cuando la funcion change context vea que no hay tarea pendiente de ejecucion va a cargar la tarea idle
        change_context();
    }

    __asm__ __volatile__ ("sti");   
    outb(0x60, 0x20);               //EOI para que vuelva a interrumpir el timer tick

    //Restauro los valores de los registros de proposito general
    __asm__ __volatile__ ("pop %esi");
    __asm__ __volatile__ ("pop %edi");
    __asm__ __volatile__ ("pop %edx");
    __asm__ __volatile__ ("pop %ecx");
    __asm__ __volatile__ ("pop %ebx");
    __asm__ __volatile__ ("pop %eax");
    __asm__ __volatile__ ("leave");
    __asm__ __volatile__ ("iret");
    return 1;
}

__attribute__(( section(".interrupt_code"))) byte default_interrupt(){
    /*Todas las interrupciones que no tengan asinada una funcion caen aca*/
    __asm__ __volatile__ ("xchg %bx, %bx");
    __asm__ __volatile__ ("hlt");
    __asm__ __volatile__ ("leave");
    __asm__ __volatile__ ("iret");
    return 1;
}



/* Direcciones de memoria con la ubicacion de los handlers
0x000000000010003c                handler_AC
0x0000000000100050                handler_SS
0x0000000000100064                handler_UD
0x0000000000100078                handler_DF
*/

__attribute__(( section(".interrupt_code"))) byte handler_AC(){
    //Deberia popear el ERROR CODE aunque sabemos que va a ser cero siempre
    __asm__ __volatile__ ("xchg %bx, %bx");
    __asm__ __volatile__ ("hlt");
    __asm__ __volatile__ ("leave");
    __asm__ __volatile__ ("iret");
    return 1;
}

__attribute__(( section(".interrupt_code"))) byte handler_SS(){
    //Deberia popear el ERROR CODE
    __asm__ __volatile__ ("xchg %bx, %bx");
    __asm__ __volatile__ ("hlt");
    __asm__ __volatile__ ("leave");
    __asm__ __volatile__ ("iret");
    return 1;
}

__attribute__(( section(".interrupt_code"))) byte handler_UD(){
    //No hay error code
    __asm__ __volatile__ ("xchg %bx, %bx");
    __asm__ __volatile__ ("hlt");
    __asm__ __volatile__ ("leave");
    __asm__ __volatile__ ("iret");
    return 1;
}

__attribute__(( section(".interrupt_code"))) byte handler_DF(){
    //Deberia popear el ERROR CODE aunque sabemos que va a ser cero siempre
    __asm__ __volatile__ ("xchg %bx, %bx");
    __asm__ __volatile__ ("hlt");
    __asm__ __volatile__ ("leave");
    __asm__ __volatile__ ("iret");
    return 1;
}

__attribute__(( section(".interrupt_code"))) byte handler_GP(){
    //Deberia popear el ERROR CODE
    __asm__ __volatile__ ("xchg %bx, %bx");
    __asm__ __volatile__ ("hlt");
    __asm__ __volatile__ ("leave");
    __asm__ __volatile__ ("iret");
    return 1;
}

__attribute__(( section(".interrupt_code"))) __attribute__((naked)) byte handler_PF(){
    //Deberia popear el ERROR CODE
    //leo el CR2 y tiro por pantalla que direccion me produjo el page fault
    __asm__ __volatile__ ("pop %edx");
    //Rescato estos registros en la pila para no romper el programa cuando vuelve de la interrupcion
    __asm__ __volatile__ ("push %eax");
    __asm__ __volatile__ ("push %ebx");
    __asm__ __volatile__ ("push %esi");
    __asm__ __volatile__ ("push %edi");
    dword ERROR_CODE=0x0;
    __asm__ __volatile__ ("mov %%edx, %0": "=r"(ERROR_CODE));

    dword cr2=0x0;
    asm("mov %%cr2,%0" : "=r"(cr2));

    dword cr3=0x0;
    asm("mov %%cr3,%0" : "=r"(cr3));

    print_error(ERROR_CODE, cr2);       //Se implementa una funcion para imprimir el texto ya que llenaba la pila de porqueria y era dificil el debug

    if(ERROR_CODE & 0x01){
        //Si esta prendido el bit P es porque no fue un error de pagina no presente
        //No puedo hacer nada...
        __asm__ __volatile__ ("cli");
        while(1){
            __asm__ __volatile__ ("hlt");
        }
        __asm__ __volatile__ ("iret");
    }

    dword dtpe = 0;
    //creo la pagina faltante
    //Primero me fijo si ya esta cargarda la DTP
    if(!test_DTP(cr3, EXTRACT_DTP_OFFSET(cr2))){
        //No esta cargada la DTP con ese offset
        __create_page_table(cr3, EXTRACT_DTP_OFFSET(cr2), __get_new_random_page(), 0, 0, 0, 1);
    }
    //Haya creado o ya existia la TP, cargo la entrada
    dtpe = *((dword*)(cr3 + EXTRACT_DTP_OFFSET(cr2)*4));

    //Ahora que tengo ubicada la TP me fijo de cargar la pagina
    //Cuando yo haga el test, puede que me vuelva a saltar otro PF ya que si la TP la acabo de crear, tiene asignada direccion fisica pero no esta cargado el identity maping
    if(!test_TP(dtpe, EXTRACT_TP_OFFSET(cr2))){
        //No esta cargada la TP con ese offset
        //Analizo dos situaciones distintas.. 
        //Podria ser que sea un PF genuino o podria ser un PF producido por una nueva pagina en la seccion de paginas random.
        if((cr2 & 0xFFC00000) == 0x0A000000){
            //Si se cumple esta condicion es porque estoy en los 4Mb de paginas random que voy a asignar por identity mapping
            __create_page(dtpe, EXTRACT_TP_OFFSET(cr2), cr2, 0, 1, 0, 0, 1);
        }else{
            __create_page(dtpe, EXTRACT_TP_OFFSET(cr2), __get_new_random_page(), 0, 1, 0, 0, 1);
        }
    }

    //Devuelvo el contenido a los registros
    __asm__ __volatile__ ("pop %edi");
    __asm__ __volatile__ ("pop %esi");
    __asm__ __volatile__ ("pop %ebx");
    __asm__ __volatile__ ("pop %eax");
    __asm__ __volatile__ ("iret");
    return 1;
}

__attribute__(( section(".interrupt_code"))) byte print_error(dword ERROR_CODE, dword cr2){
    //Matrix de los caracteres HEXA
    const char ascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    
    //Primer linea del mensaje de error
    const char line1[28] = "ERROR: Page fault detected.";
    printlog(line1);
    
    //String constantes para los diferentes mensajes de error
    const char line_P[18]    = "Page not present.";
    const char line_WR[26]   = "Writing a read-only page.";
    const char line_US[44]   = "Trying to access a supervisor page as user.";
    const char line_RSVD[26] = "Bit set in reserved area.";
    const char line_ID[36]   = "Cause of PF was a intruction fetch.";
    const char line_PK[77]   = "Cause of PF was a data access to a user page with protection key disallowed.";
    const char line_SGX[24]  = "PF unrelated to paging.";

    //Miro el flag P (0x01)
    if(!(ERROR_CODE&0x01)){
        printlog(line_P);
    }
    //Miro el flag W/R
    if(ERROR_CODE&0x02){
        printlog(line_WR);
    }
    //Miro el flag U/S
    if(ERROR_CODE&0x04){
        printlog(line_US);
    }
    //Miro el flag RSVD
    if(ERROR_CODE&0x08){
        printlog(line_RSVD);
    }
    //Miro el Flag I/D
    if(ERROR_CODE&0x10){
        printlog(line_ID);
    }
    //Miro el flag PK
    if(ERROR_CODE&0x20){
        printlog(line_PK);
    }
    //Miro el flag SGX
    if(ERROR_CODE&0x80){
        printlog(line_SGX);
    }

    //Preparo la linea donde digo cual fue la direccion lineal que tiro error
    char line3[22] = "Lin. Addr: 0x";
    

    for(byte i=0;i<8;i++){
        line3[13+i] = ascii[(cr2>>(28-4*i))&0xF];
    }

 
    printlog(line3);
    return 1;
}