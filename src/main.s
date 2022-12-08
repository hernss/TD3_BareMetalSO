USE32

%include "./inc/x86_types.h"

section .start32

EXTERN CS_SEL
EXTERN DS_SEL

EXTERN __STACK_END_32
EXTERN __STACK_SIZE_32

EXTERN __PILA_KERNEL

EXTERN __KERNEL_32_LMA
EXTERN __KERNEL_32_PHI
EXTERN __codigo_kernel32_size

EXTERN __INTERRUPTS_LMA
EXTERN __INTERRUPTS_PHI
EXTERN __codigo_interrupts_size

EXTERN __FUNCTIONS_LMA
EXTERN __FUNCTIONS_VMA
EXTERN __functions_size

EXTERN __TAREA_1_TEXT_PHI
EXTERN __TAREA_1_BSS_PHI
EXTERN __TAREA_1_RODATA_PHI
EXTERN __TAREA_1_DATA_PHI

EXTERN __TAREA_1_TEXT_LMA
EXTERN __TAREA_1_RODATA_LMA
EXTERN  __TAREA_1_BSS_LMA
EXTERN  __TAREA_1_DATA_LMA

EXTERN __tarea_1_text_size
EXTERN __tarea_1_rodata_size
EXTERN __tarea_1_bss_size
EXTERN __tarea_1_data_size

EXTERN __TAREA_2_TEXT_PHI
EXTERN __TAREA_2_BSS_PHI
EXTERN __TAREA_2_RODATA_PHI
EXTERN __TAREA_2_DATA_PHI

EXTERN __TAREA_2_TEXT_LMA
EXTERN __TAREA_2_RODATA_LMA
EXTERN __TAREA_2_BSS_LMA
EXTERN __TAREA_2_DATA_LMA

EXTERN __tarea_2_text_size
EXTERN __tarea_2_rodata_size
EXTERN __tarea_2_bss_size
EXTERN __tarea_2_data_size

EXTERN __TAREA_3_TEXT_PHI
EXTERN __TAREA_3_BSS_PHI
EXTERN __TAREA_3_RODATA_PHI
EXTERN __TAREA_3_DATA_PHI

EXTERN __TAREA_3_TEXT_LMA
EXTERN __TAREA_3_RODATA_LMA
EXTERN __TAREA_3_BSS_LMA
EXTERN __TAREA_3_DATA_LMA

EXTERN __tarea_3_text_size
EXTERN __tarea_3_rodata_size
EXTERN __tarea_3_bss_size
EXTERN __tarea_3_data_size

EXTERN __TAREA_4_TEXT_PHI
EXTERN __TAREA_4_BSS_PHI
EXTERN __TAREA_4_RODATA_PHI
EXTERN __TAREA_4_DATA_PHI

EXTERN __TAREA_4_TEXT_LMA
EXTERN __TAREA_4_RODATA_LMA
EXTERN __TAREA_4_BSS_LMA
EXTERN __TAREA_4_DATA_LMA

EXTERN __tarea_4_text_size
EXTERN __tarea_4_rodata_size
EXTERN __tarea_4_bss_size
EXTERN __tarea_4_data_size

EXTERN __fast_memcpy_rom
EXTERN create_gdt
EXTERN create_idt
EXTERN _gdtr_c
EXTERN _idtr_c

EXTERN inicializar_paginacion
EXTERN clear
EXTERN init_keyboard
EXTERN inicializar_scheduler

CS_SEL_C equ 0x08
DS_SEL_C equ 0x10
TSS_SEL_C equ 0x18
CS_SEL_C_USER equ 0x20
DS_SEL_C_USER equ 0x28

GLOBAL start32_launcher
GLOBAL kernel32_init

GLOBAL save_context
GLOBAL load_context

EXTERN actual_context_id
EXTERN last_context_id

EXTERN trueTSS
EXTERN null_data

;MMX context data
EXTERN mmx_context_data
EXTERN __MMX_DATA_T1_PHI
EXTERN __MMX_DATA_T2_PHI
EXTERN __MMX_DATA_T3_PHI
EXTERN __MMX_DATA_T4_PHI

start32_launcher:

    mov ax, DS_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ;Preparo la pila temporal
    mov ss, ax
    mov esp, __STACK_END_32
    xor eax, eax

    ;Ahora que tengo un mini modo protegido con una precaria GDT en ROM voy a hacer una memory test
    ;Si el test esta OK continuo con la carga de la GDT de RAM

    ;Asumo que el sistema tiene 1GB de RAM
    ;MAX Memory ADDRESS 0x3FFFFFFF
    ;Total del test: 4.716.867 ciclos de clock calculado con el time stamp counter del procesador
    xor edi, edi

    rdtsc
    mov ecx, eax

mem_test_loop:
    mov dword [edi], 0xAAAAAAAA
    mov eax, [edi]
    cmp eax, 0xAAAAAAAA
    jne mem_test_error
    mov dword [edi], 0x55555555
    mov eax, [edi]
    cmp eax, 0x55555555
    jne mem_test_error
    mov dword [edi], 0x0
    mov eax, [edi]
    cmp eax, 0x0
    jne mem_test_error
    add edi, 0x1000
    cmp edi, 0xA0000    ;Aca empiezan datos de la bios y mas abajo el shadow de la rom
    jne next
    mov edi, 0x100000   ;entoces avanzo hasta despues del shadow de la rom y en el medio "confio" que anda bien.
next:
    cmp edi, 0x40000000 ;Si llegue al final del primer GB
    je mem_test_end     ;Salto al final del test
    jmp mem_test_loop

mem_test_error:
    cli
    hlt
    jmp mem_test_error          ;Adios mundo cruel

mem_test_end:
    ;Si llegue hasta aca es porque el memory test fue exitoso.

    rdtsc
    sub eax, ecx                    ;ECX tiene la cantidad de ciclos que tomo el memory test


    ;Ahora chequeo que el CPU tenga las especificaciones que necesito para mi programa
    ; Step 1.Execute the CPUID instruction with an input value of EAX=0 and ensure the vendor-ID string returned is "GenuineIntel".
    ; Step 2.Execute the CPUID instruction with EAX=1 to load the EDX register with the feature flags.
    
    mov eax, 0
    cpuid

    ;Chequeo que sea un "GenuineIntel"
    cmp ebx, x86_CPUID_GenuineIntel_EBX
    jne .guard
    cmp ecx, x86_CPUID_GenuineIntel_ECX
    jne .guard
    cmp edx, x86_CPUID_GenuineIntel_EDX
    jne .guard                  

    mov eax, 1
    cpuid

    test edx, X86_CPUID_FPU
    jz .guard                       ;No tengo FPU.. sin FPU no hay paraiso
    test edx, X86_CPUID_MMX
    jz .guard                       ;No tengo instruciones SIMD
    test edx, X86_CPUID_FXSR
    jz .guard                       ;No tengo las instrucciones rapidas para guardar y recuperar la pila.
    test edx, X86_CPUID_SSE
    jz .guard                       ;No tengo el set de instrucciones SSE
    test edx, X86_CPUID_SSE2
    jz .guard                       ;No tengo el set de instrucciones SSE2

    ;Preparo la GDT en RAM
    push ebp                        ;ENTER
    mov ebp, esp
    call create_gdt
    leave

    lgdt [_gdtr_c]    ;Cargo la gdt que acabo de crear en C en RAM+

    ;Ahora que tengo cargada la nueva GDT en RAM actualizo los selectores de segmento

    mov ax, DS_SEL_C
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ss, ax                      ;Cargo el segmento de pila
    mov esp, __PILA_KERNEL + 0xFFF  ;Cargo el puntero de pila, __PILA_KERNEL apunta al comienzo de la pagina donde voy a guardar la pila de kernel

    mov ax, TSS_SEL_C              ;Una vez que cree mi trueTSS en la funcion create_gdt
    ltr ax                         ;Cargo el registro tr con la instruccion ltr

    push ebp                        ;ENTER
    mov ebp, esp 
    call create_idt                 ;Creo la tabla de interrupciones
    leave

    lidt [_idtr_c]

    push ebp                        ;ENTER
    mov ebp, esp
    call configure_PIC              ;Envio la secuencia de inicializacion del PIC
    leave

    push ebp                        ;ENTER
    mov ebp, esp
    push __codigo_interrupts_size     ;Copio el codigo de los handlers de interrupciones
    push __INTERRUPTS_PHI
    push __INTERRUPTS_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard 

    ;Copio el codigo del programa
    push ebp                        ;ENTER
    mov ebp, esp
    push __codigo_kernel32_size     ;Copio el codigo del kernel
    push __KERNEL_32_PHI
    push __KERNEL_32_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    ;Copio la Tarea 1 a memoria
    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_1_text_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_1_TEXT_PHI
    push __TAREA_1_TEXT_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_1_rodata_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_1_RODATA_PHI
    push __TAREA_1_RODATA_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_1_bss_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_1_BSS_PHI
    push __TAREA_1_BSS_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_1_data_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_1_DATA_PHI
    push __TAREA_1_DATA_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    ;Copio la Tarea 2 a memoria
    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_2_text_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_2_TEXT_PHI
    push __TAREA_2_TEXT_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_2_rodata_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_2_RODATA_PHI
    push __TAREA_2_RODATA_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_2_bss_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_2_BSS_PHI
    push __TAREA_2_BSS_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_2_data_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_2_DATA_PHI
    push __TAREA_2_DATA_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    ;Copio la Tarea 3 a memoria
    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_3_text_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_3_TEXT_PHI
    push __TAREA_3_TEXT_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_3_rodata_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_3_RODATA_PHI
    push __TAREA_3_RODATA_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_3_bss_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_3_BSS_PHI
    push __TAREA_3_BSS_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_3_data_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_3_DATA_PHI
    push __TAREA_3_DATA_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    ;Copio la tarea 4 a memoria
    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_4_text_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_4_TEXT_PHI
    push __TAREA_4_TEXT_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_4_rodata_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_4_RODATA_PHI
    push __TAREA_4_RODATA_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_4_bss_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_4_BSS_PHI
    push __TAREA_4_BSS_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard

    push ebp                        ;ENTER
    mov ebp, esp
    push __tarea_4_data_size     ;Copio el codigo de los handlers de interrupciones
    push __TAREA_4_DATA_PHI
    push __TAREA_4_DATA_LMA
    call __fast_memcpy_rom
    leave
    cmp eax, 1
    jne .guard


    ;estas secciones de memoria me conviene inicializarlas ahora antes de prender la paginacion
    mov eax, __MMX_DATA_T1_PHI
    fxsave [eax]
    mov eax, __MMX_DATA_T2_PHI
    fxsave [eax]
    mov eax, __MMX_DATA_T3_PHI
    fxsave [eax]
    mov eax, __MMX_DATA_T4_PHI
    fxsave [eax]

    ;Activo la paginacion
    push ebp                        ;ENTER
    mov ebp, esp

    call inicializar_paginacion
    leave

    mov cr3, eax                                                ;;Cargo el CR3 que acabo de generar en inicializar paginacion

    mov eax, cr0
    or eax, X86_CR0_PG | X86_CR0_WP | X86_CR0_PE | X86_CR0_AM   ;Activo la paginacion, el Write Protect, el aligment check y la proteccion

    xor ebx, ebx
    or ebx, (X86_CR0_NW | X86_CR0_CD)
    not ebx                                                     ;Tengo que poner en 0 los bit del Not Write Throught y Cache Disabled para habilitar la cache

    and eax, ebx                                                ;ebx voy a tener todos 1's menos en los bits CD y NW que voy a tener 0's  
    mov cr0, eax
    
    mov eax, cr4
    or eax, X86_CR4_OSXMMEXCPT | X86_CR4_OSFXSR | X86_CR4_PGE   ;Activo la excepcion NM cuando uso la FPU y aprovecho para activar las paginas globales
    mov cr4, eax


    ;seteo los id de contextos a FFFF para marcar que todavia no se ejecuto ninguna tarea
    mov eax, last_context_id
    mov word [eax], 0xFFFF
    mov eax, actual_context_id
    mov word [eax], 0xFFFF


    mov eax, null_data
    mov dword [eax], 0x0
    mov dword [eax+4], 0x0

    jmp CS_SEL_C:kernel32_init                                  ;Salto al codigo del kernel que acabo de copiar

.guard:
    cli
    hlt 
    jmp .guard


configure_PIC:
    ;ICW1 0001 0001 
    mov al, 0x11
    out 0x20, al
    out 0xA0, al

    ;ICW2 
    mov al,0x20
    out 0x21, al

    mov al, 0x28
    out 0xA1, al

    ;ICW3
    mov al, 0x04
    out 0x21, al

    mov al, 0x02
    out 0xA1, al

    ;ICW4
    mov al, 0x01
    out 0x21, al
    out 0xA1, al

    ;Activo la int del teclado y del timer tick
    mov al, 11111100b
    out 0x21, al

    ;Enmascaro las otras ints
    mov al, 0xFF
    out 0xA1, al

    ;Configuro el timer tick
    ;Freq = 1.193182 MHz
    ;Si quiero configurarlo para tener una interrupcion cada 10ms tengo que contar 10ms * 1.193182MHz = 11931.85 cuentas.. 
    ;11932 = 0x2E9C
    

    mov al,00110100b               ;channel 0, lobyte/hibyte, rate generator
    out 0x43, al

    mov ax, 0x2E9C                 ;Cargo en AX el valor de recarga
    out 0x40,al                    ;Mando al PIT los bits menos significativos
    mov al,ah                      ;paso los bits mas significativos a al
    out 0x40,al                    ;Mando al pit los bits mas significativos que tengo cargados en al

    ret


section .kernel32

kernel32_init:

    call clear                      ;Inicializo la pantalla y borro
    call init_keyboard              ;inicializo el teclado
    call inicializar_scheduler      ;Inicializo el scheduler
    sti                             ;Habilito las interrupciones para poner en marcha el timer tick
kernel32_loop:
    hlt
    jmp kernel32_loop               ;Bucle eterno hasta que me desconecten


section .functions_ram

save_context:
    mov eax, [esp+4]            ;Direccion base del contexto parametro pasado por pila
    
    mov edx, [esp+52]           ;;ESI
    mov [eax+16], edx
    mov edx, [esp+56]           ;;EDI
    mov [eax+20], edx
    mov edx, [esp+60]           ;;EDX
    mov [eax+12], edx
    mov edx, [esp+64]           ;;ECX
    mov [eax+8], edx
    mov edx, [esp+68]           ;;EBX
    mov [eax+4], edx
    mov edx, [esp+72]           ;;EAX
    mov [eax], edx

    mov dx, [esp+108]  ;;CS      ;Guardo los selectores en el contexto
    mov [eax+28], dx
    mov dx, ds 
    mov [eax+30], dx
    mov dx, es 
    mov [eax+32], dx
    mov dx, fs 
    mov [eax+34], dx
    mov dx, gs 
    mov [eax+36], dx

    mov edx, [esp+100]           ;;EBP
    mov [eax+24], edx           ;guardo el ebp

    mov edx, esp                 ;La pila actual es la de nivel 0
    add edx, 116                 ;Guardo la posicion de la pila sin ningun registro pusheado ya que cuando levante el contexto le voy a pushear todos los datos y las direcciones de retorno.
    mov [eax+40], edx

    mov dx, ss
    mov [eax+38], dx

    mov edx, cr3                ;Guardo la paginacion de la tarea actual
    mov [eax+52], edx           ;aunque no seria necesario ya que el cr3 no deberia cambiar

    mov edx, [esp+104]           ;;EIP
    mov [eax+56], edx

    mov edx, [esp+112]           ;;EFLAGS
    mov [eax+60], edx

    ;Save context se da en la situacion de una tarea que estaba ejecutandose y la estoy a punto de sacar de ejecucion
    ;Miro el flag_mmx, si esta en 1 es porque durante su ejecucion utilizo la FPU y en el handler de la excepcion NM restauro su contexto 
    ;Entonces ahora que estoy a punto de desalojarla del micro guardo su contexto
    ;Despues de guardar el contexto bajo el flag porque de esta manera si en el proximo ciclo de ejecucion de casualidad no usa la FPU entonces me salto la parte de guardar el contexto de mmx
    mov edx, [eax+68]           ;Leo el flag_mmx           
    cmp edx, 0x01               ;si es 1 es porque use la FPU 
    jne fin_save_context

    clts                        ;por las dudas limpio el flag, no sea cosa que me haya quedado arriba y salta la excepcion NM

    mov edx, mmx_context_data
    fxsave [edx]  
    xor edx, edx
    mov dword [eax+68], edx           ;Pongo el flag en 0

fin_save_context:
    ret

load_context:
    mov edx, [esp+8]            ;CR3 pasado por parametro
    mov eax, [esp+4]            ;Direccion base del contexto parametro pasado por pila
    
    
    ;;Rescato los valores importantes de la pila para recargar los registros de proposito general que pushie en la pila al inicio del pit0
    ;;La idea es que cuando termine el pit0 recupere estos valores en los registro EAX--ESI

    pop ebx ;; EIP que devuelve a change context

    add esp,40    ;;Me salto las variables locales reservadas por el gcc
    pop ecx ;; (EBP)


    pop edi ;; EIP que devuelve al pit0


    add esp, 48  ;;Me salto los registro de proposito general de la tarea anterior que estaban salvados en la pila.

    pop esi ;; EBP

    add esp, 12  ;;Me salto los registro del iret viejos

    ;mov edx, esp                ;La pila actual es la de nivel 0
    ;mov [eax+40], edx

    ;mov dx, ss
    ;mov [eax+38], dx



    ;Primero que nada cambio el cr3 porque sino no puedo acceder al contexto de la tarea ya que la pagina donde guardo el contexto solo la tengo paginada en cada tarea
    ;;Ahora que vacie la pila hago el cambio de cr3
    mov cr3, edx

    mov dx, [eax+30]            ;cargo los selectores en el contexto
    mov ds, dx 
    mov dx, [eax+32]
    mov es, dx 
    mov dx, [eax+34]
    mov fs, dx 
    mov dx, [eax+36]
    mov gs, dx

    ;Levanto el flag de task switch
    mov edx, cr0
    xor edx, X86_CR0_TS
    mov cr0, edx

    ;;Reconstruyo la pila para la nueva tarea
    ;Primero me traigo la pila del contexto
    xor edx, edx
    mov dx, [eax+38]
    mov ss, dx

    mov edx, [eax+40]
    mov esp, edx                ;Traigo la pila nivel 0 del contexto
    
    ;;Ahora voy a restaurar el contexto para cargar, Podria estar en la situacion que tengo salto de nivel o que el cambio de contexto vino desde la syscall por ende tengo que volver sin cambio de prioridad
    xor edx, edx
    mov dx, [eax+28]           ;;CS a donde tengo que hacer el iret del SYSTICK
    test dx, CS_SEL_C
    jnz .sin_cambio_pl          ;;  Si no es igual al CS de nivel cero entonces estoy en la situacion de cambio de privilegio


    ;Ahora que tengo la pila correcta le cargo los valores, algunos desde el contexto y otros desde los valores de ejecucion como los eip de retorno que estaban en la pila
    xor edx, edx
    mov dx, [eax+44]
    or dx, 0x3                  ;;Seteo el RPL en nivel 3
    push edx                    ;;SS nivel 3

    mov edx, [eax+48]
    push edx                    ;;ESP nivel 3
        
    mov edx, [eax+60]           ;;EFLAGS
    push edx

    xor edx, edx
    mov dx, [eax+28]           ;;CS
    or dx, 0x3                  ;;Seteo el RPL en nivel 3
    push edx
    jmp .end_cs

.sin_cambio_pl:

    mov edx, [eax+60]           ;;EFLAGS
    push edx

    xor edx, edx
    mov dx, [eax+28]           ;;Pusheo el CS sin cambio de privilegio
    push edx


.end_cs:    
    mov edx, [eax+56]           ;;EIP
    push edx

    mov edx, [eax+24]           ;;EBP
    push edx                ;guardo el ebp

    sub esp,0x18            ;;Variables locales de pit0

    mov edx, [eax]           ;;EAX
    push edx

    mov edx, [eax+4]           ;;EBX
    push edx

    mov edx, [eax+8]           ;;ECX
    push edx

    mov edx, [eax+12]           ;;EDX
    push edx

    mov edx, [eax+20]           ;;EDI
    push edx

    mov edx, [eax+16]           ;;ESI
    push edx

    push edi ;; EIP que devuelve al pit0

    ;Ahora me toca cargar el EBP que pusheo cuando entro a change context.
    ;Este valor es el que va a cargar en el ESP cuando haga el leave
    ;es por esto que lo calculo segun el nuevo esp ya que este valor no lo tengo resguardado en ningun lado
    ;cabe aclarar que las variables locales de los procesos padre no son validas ya que las variables locales se guardan en la pila
    
    ;El ebp de la pila depende de si hay cambio de privilegio o no
    xor edx, edx
    mov dx, [eax+28]           ;;CS a donde tengo que hacer el iret del SYSTICK
    test dx, CS_SEL_C
    jnz .ebp_sin_cambio_pl          ;;  Si no es igual al CS de nivel cero entonces estoy en la situacion de cambio de privilegio
    
    mov edx, [eax+40]
    sub edx, 0x18 
    push edx   ;; EBP reconstruido segun la nueva pila
    jmp .end_ebp

.ebp_sin_cambio_pl:
    
    mov edx, [eax+40]
    sub edx, 0x10 
    push edx   ;; EBP reconstruido segun la nueva pila

.end_ebp:
    
    ;emulo que lo hubiese hecho el gcc en la entrada a la funcion
    mov ebp, esp
 
    sub esp,0x28    ;;Variables locales de change context
    ;push ecx ;; CS que devuelve a change context
    push ebx ;; EIP que devuelve a change context

    ;cambio el id del proceso actual
    mov dx, [eax+64]           ;;context id
    mov word [actual_context_id], dx

    ;Actualizo el ESP0 y el SS0 en la trueTSS
    mov edx, trueTSS
    add edx, 4
    mov ebx, [eax+40]
    or ebx, 0xFFF           ;Me voy a la base de la pila
    mov dword [edx], ebx   ;Cargo el ESP0 en la trueTSS
    add edx, 4
    mov bx, [eax+38]
    mov word [edx], bx    ;Cargo el SS0 en la trueTSS
    ret
