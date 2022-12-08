#include "../inc/prototipos.h"

__attribute__(( section(".datos_user"))) __attribute__ ((aligned (8))) qword promedio = 0;
__attribute__(( section(".datos_user"))) __attribute__ ((aligned (8))) qword null_data = 0;
__attribute__(( section(".t1_bss"))) __attribute__ ((aligned (8))) qword aux_t1;
__attribute__(( section(".t1_bss"))) __attribute__ ((aligned (8))) qword acc_t1;

//Prototipos de funciones locales
qword udivdi3 (qword dividend, qword divisor);
int clzll(qword d);
int ctzll(qword d);

__attribute__ ((aligned (8))) byte leer_address(byte *addr){
    static dword last_dir = 0;
    if( (dword)addr & 0xE0000000){
        //se pasa de los 512MB
        char str[38] = "La direccion lineal excede los 512Mb.";

        if(last_dir != (dword)addr){
            __asm__ __volatile__ ("mov %0, %%ebx": : "r"(&str));
            __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_PRINT));
            __asm__ __volatile__ ("int $0x80");         //Hago la syscall para imprimir en pantalla
            last_dir = (dword)addr;
        }
        return 0;
    }
    byte a = *addr;
    return a;
}

__attribute__ ((aligned (8))) void Tarea_promedio(){
    while(1){
        word item_count;

        __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_READ_COUNT));
        __asm__ __volatile__ ("int $0x80");         //Hago la syscall para leer la cantidad de datos que tengo en la tabla ya que es una variable que maneja el kernel
        __asm__ __volatile__ ("mov %%ax, %0": "=m"(item_count));

        //En realidad item count no es la cantidad sino el offset en la tabla porque es una variable que se utiliza para ir guardando de a bytes en la tabla
        item_count >>= 3;        //Divido table offset por 8 para obtener la cantidad de elementos que tiene la table

        if(item_count!=0){
            acc_t1=0;
            aux_t1=0;

            for(word i = 0; i < item_count; i++){
                __asm__ __volatile__ ("mov %0, %%ebx": : "r"(&aux_t1));
                __asm__ __volatile__ ("mov %0, %%cx": : "r"(i));
                __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_READ));         //Leo la posicion i de la tabla de digitos y lo almaceno en aux_t1 para luego acumularla
                __asm__ __volatile__ ("int $0x80");                                 
                                                        
                acc_t1 += aux_t1;
            }
            
            promedio = udivdi3(acc_t1,item_count);

            //leer_address((byte *)(dword)promedio);
        }
        __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_SHOW_AVG));
        __asm__ __volatile__ ("int $0x80");         //Hago la syscall para forzar el cambio de contexto, cuando tenga prioridades este cambio se hace a nivel kernel
        __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_TASK_SWITCH));
        __asm__ __volatile__ ("int $0x80");         //Hago la syscall para forzar el cambio de contexto, cuando tenga prioridades este cambio se hace a nivel kernel
        __asm__ __volatile__ ("mov %0, %%eax": : "r"(INT_80_HLT));
        __asm__ __volatile__ ("int $0x80");         //Hago la syscall para irme a dormir
    }
    return;
}

/* __udivdi3 - 64 bit integer unsigned divide  */
__attribute__ ((aligned (8))) qword udivdi3 (qword dividend, qword divisor)
{
    /* Divide out any power of two factor from dividend and divisor.
        Note that when dividing by zero the divisor will remain zero,
        which is all we need to detect that case below.  */
    const int power_of_two_factor = ctzll (divisor);
    divisor >>= power_of_two_factor;
    dividend >>= power_of_two_factor;
    /* Checks for division by power of two or division by zero.  */
    if (divisor <= 1)
    {
        if (divisor == 0)
        {
            //raise_intdiv ();  //Aca deberia saltar una division por 0..
            return 0;
        }
        return dividend;
    }


    if (((dword) (dividend >> 32) | ((dword) (divisor >> 32))) == 0)
    {
        /* Operands both fit in 32 bits, so use faster 32 bit algorithm.  */
        return (dword)dividend/(dword)divisor;
    }


    /* See algorithm description in __udivsi3  */

    const int divisor_clz = clzll (divisor);
    const qword max_divisor = divisor << divisor_clz;
    const qword max_qbit = 1ULL << divisor_clz;

    qword quotient = 0;
    qword remainder = dividend;

    while (remainder >= divisor)
    {
        int shift = clzll (remainder);
        qword scaled_divisor = max_divisor >> shift;
        qword quotient_bit = max_qbit >> shift;

        int too_big = (scaled_divisor > remainder);
        scaled_divisor >>= too_big;
        quotient_bit >>= too_big;
        remainder -= scaled_divisor;
        quotient |= quotient_bit;
    }
    return quotient;
}

__attribute__ ((aligned (8))) int clzll(qword d){
    if(!d) return 64;
    int c_aux = 0;
    dword* dp = (dword*)(&d);
	dword dh=*(dp+1);
	if(!dh){
	    c_aux=32;
	    //Ahora me fijo la parte menos significativa
	    dh = (dword)d;
	}
	int c=0;
    while(! (  dh&   ~((1<<(31-c))-1)    ) )c++;
    
    return c + c_aux;
}

__attribute__ ((aligned (8))) int ctzll(qword d){
    if(!d) return 64;
	int c=0;
	dword dl;
	if(!((dword)d)){
		c=32;
		dword* dp = (dword*)(&d);
		dp++;
		dl = *dp;
		//Continuo por los 32bit mas significativos
	} else {
		dl = (dword)d;
	}
	while(!(dl&((1<<(c+1))-1)))c++;
	return c;
}