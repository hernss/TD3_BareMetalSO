USE32
section .text 
GLOBAL sum_sq
;; Funcion que hace la suma saturada de word
;; param 1 - direccion base de la lista de 64bits
;; param 2 - cantidad de elementos
;; param 3 - direccion base para almacenar el resultado
;; no retorna nada
EXTERN null_data

sum_sq:
    ;almaceno en esp+4 el resultado de sumar esp+4 con esp+8
    mov eax, [esp+4]     ;Direccion del operando destino
    mov edx, [esp+8]     ;Direccion del operando source


    movq mm0, [eax]
    movq mm1, [edx]
    paddq mm0, mm1    ;Hago la suma saturada y guarda el resultado en mm0
    movq [eax], mm0     ;Muevo los 64bits a la posicion que me pasaron por la pila
    ret
