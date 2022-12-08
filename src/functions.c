#include "../inc/prototipos.h"

__attribute__(( section(".interrupt_code"))) unsigned char inb (unsigned short int __port)
{
  unsigned char _v;
  __asm__ __volatile__ ("inb %w1,%0":"=a" (_v):"Nd" (__port));
  return _v;
}

__attribute__(( section(".interrupt_code"))) void outb (unsigned char __value, unsigned int __port)
{
  __asm__ __volatile__ ("outb %b0,%w1": :"a" (__value), "Nd" (__port));
}

__attribute__(( section(".functions_rom"))) byte __fast_memcpy_rom ( byte *src, byte *dst, dword length){

    byte status = ERROR_DEFECTO;

    if(length == 0) return EXITO;     //Nada que copiar
    if(length > 0){
        while(length)
        {
            length--;
            *dst++ = *src++;
        }
        status = EXITO;
    }

    return (status);

}




