USE32

section .interrupt_code

EXTERN mostrar_suma_sw
EXTERN mostrar_suma_sq
EXTERN mostrar_promedio
EXTERN flag_change_context
EXTERN printlog

EXTERN read_bytes_count
EXTERN read_buffer
EXTERN table_offset

INT_80_HLT          equ 1
INT_80_TASK_SWITCH  equ 2
INT_80_SHOW_ADD_SW  equ 3
INT_80_SHOW_ADD_SQ  equ 4
INT_80_SHOW_AVG     equ 5
INT_80_PRINT        equ 6
INT_80_READ         equ 7
INT_80_READ_COUNT   equ 8

BASE_TABLA_DIGITOS  equ 0x01210000


GLOBAL handler_INT80

handler_INT80:

;    push eax                       ;no puedo pushearlo porque lo voy a usar para devolver el resultado
    push ebx
    push ecx
    push edx
    push edi

    sti

    cmp eax, INT_80_HLT
    jne .test_ts
    hlt                             ;Para que las tareas de nivel 3 se puedan dormir
    jmp .end
.test_ts:
    cmp eax, INT_80_TASK_SWITCH
    jne .test_add_sw
    mov ebx, flag_change_context
    mov byte [ebx], 1               ;Fuerzo el cambio de contexto para que la tarea que esta ejecutando sea sacada de contexto en el proximo timertick

    jmp .end
.test_add_sw:
    cmp eax, INT_80_SHOW_ADD_SW
    jne .test_add_sq
    
    push ebp                        ;ENTER
    mov ebp, esp
    call mostrar_suma_sw            ;muestro en pantalla la suma satured word
    leave

    jmp .end
.test_add_sq:
    cmp eax, INT_80_SHOW_ADD_SQ
    jne .test_avg
    
    push ebp                        ;ENTER
    mov ebp, esp
    call mostrar_suma_sq            ;muestro en pantalla la suma satured qword
    leave

    jmp .end
.test_avg:
    cmp eax, INT_80_SHOW_AVG
    jne .test_print
    
    push ebp                        ;ENTER
    mov ebp, esp
    call mostrar_promedio           ;muestro en pantalla el promedio
    leave

    jmp .end
.test_print:
    cmp eax, INT_80_PRINT           ;PRINT EN PANTALLA: ebx trae el puntero al comienzo de la string
    jne .test_read
    push ebp                        ;ENTER
    mov ebp, esp
    push ebx
    call printlog                   ;Escribo la cantidad de bytes que me pasaron por ECX del buffer EBX a la pantalla
    leave

    jmp .end
.test_read:
    cmp eax, INT_80_READ
    jne .test_read_count
    ;Leo la posicion de la tabla en ECX y almaceno en la direccion que me pasaron por EBX
    ;Primero chequeo que EBX apunte a una direccion de usuario
    mov edx, ebx
    and edx, 0xFFC00000     ;Calculo el DTP offset
    shr edx, 22             ;Ahora tengo en edx la posicion de la DTPE

    shl edx, 2              ;Multiplico por 4 para calcular el offset en RAM
    mov edi, cr3
    add edi, edx            ;Se lo sumo al CR3
    mov edx, [edi]
    test edx, edx           ;Primero me fijo si hay DTPE disponible.. sino es porque no esta paginada la TP y adios
    jz .error

    test edx, 0x4           ;Me fijo si la DTPE es de usuario.. sino adios
    jz .error

    and edx, 0xFFFFF000     ;Limpio los flags y voy a buscar la TP
    mov edi, edx

    mov edx, ebx
    and edx, 0x003FF000     ;Calculo el TP offset
    shr edx, 12             ;Ahora tengo en edx la posicion de la TPE

    shl edx, 2              ;Multiplico por 4 para calcular el offset en RAM    
    add edi, edx            ;Se lo sumo a la base de la TP que la tengo guardada en edi

    mov edx, [edi]
    test edx, edx           ;Primero me fijo si hay TPE disponible.. sino es porque no esta paginada la Pagina y adios
    jz .error
    test edx, 0x4           ;Me fijo si la TPE es de usuario.. sino adios
    jz .error

    ;Si llegue hasta aca es porque esta todo ok..

    mov eax, BASE_TABLA_DIGITOS
    xor edx, edx                    ;Limpio EDX
    mov dx, cx
    shl dx, 3                       ;Multiplico la posicion por 8 para calcular el offset en memoria
    add eax, edx                    ;Sumo el offset a la base de la tabla
    mov ecx, [eax]                  ;Leo el dword menos significativo
    mov [ebx], ecx
    mov ecx, [eax+4]
    mov [ebx+4], ecx                ;Leo el dword mas significativo
    jmp .end

.test_read_count:
    cmp eax, INT_80_READ_COUNT
    jne .end
    mov eax, table_offset
    mov word ax, [eax]
    jmp .end


.end:
    pop edi
    pop edx
    pop ecx
    pop ebx
;    pop eax
    iret

.error:
    mov eax, -1
    pop edi
    pop edx
    pop ecx
    pop ebx
;    pop eax
    iret