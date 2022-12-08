#include "../inc/prototipos.h"

//Pagina de KERNEL
__attribute__(( section(".datos"))) char* HEAD;
__attribute__(( section(".datos"))) char* TAIL;
__attribute__(( section(".datos"))) char buffer_circular[17];
__attribute__(( section(".datos"))) word table_offset = 0;
__attribute__(( section(".datos"))) char key_map[256];


__attribute__(( section(".functions_ram"))) void init_keyboard(){

    for(word i = 0; i<256; i++){
        key_map[i] = 0;
    }
    //BackSpace	0E / 8E
    key_map[0x02] = '1';
	key_map[0x03] = '2';
    key_map[0x04] = '3';
    key_map[0x05] = '4';
    key_map[0x06] = '5';
    key_map[0x07] = '6';
    key_map[0x08] = '7';
    key_map[0x09] = '8';
    key_map[0x0A] = '9';
    key_map[0x0B] = '0';
    key_map[0x0F] = '\t';
    key_map[0x10] = 'Q';
    key_map[0x11] = 'W';	
    key_map[0x12] = 'E';	
    key_map[0x13] = 'R';	
    key_map[0x14] = 'T';	
    key_map[0x15] = 'Y';	
    key_map[0x16] = 'U';	
    key_map[0x17] = 'I';	
    key_map[0x18] = 'O';	
    key_map[0x19] = 'P';	
    key_map[0x1E] = 'A';	
    key_map[0x1F] = 'S';	
    key_map[0x20] = 'D';	
    key_map[0x21] = 'F';	
    key_map[0x22] = 'G';	
    key_map[0x23] = 'H';
    key_map[0x24] = 'J';	
    key_map[0x25] = 'K';	
    key_map[0x26] = 'L';	
    key_map[0x1C] = '\n';
    key_map[0x2C] = 'Z';
    key_map[0x2D] = 'X';	
    key_map[0x2E] = 'C';	
    key_map[0x2F] = 'V';	
    key_map[0x30] = 'B';	
    key_map[0x31] = 'N';	
    key_map[0x32] = 'M';	
    key_map[0x39] = ' ';	
    key_map[0x4F] = '1';
	key_map[0x50] = '2';
    key_map[0x51] = '3';
    key_map[0x4B] = '4';
    key_map[0x4C] = '5';
    key_map[0x4D] = '6';
    key_map[0x47] = '7';
    key_map[0x48] = '8';
    key_map[0x49] = '9';
    key_map[0x52] = '0';

    //Inicializo los punteros del buffer circular
    HEAD = buffer_circular;
    TAIL = buffer_circular;

}

__attribute__(( section(".functions_ram"))) char get_ascii(byte keycode){
    return key_map[keycode];
}

__attribute__(( section(".functions_ram"))) void almacenar_buffer(char character){


    if( (character != '\n') && ( (character < '0') || (character>'9') ) ) return; //Filtro los caracteres que no me interesan

    if ((character == '\n') || (TAIL == &buffer_circular[17])){    
        //Si llego aca es porque el buffer ya esta en el final o vino un enter
        //Entonces, agrego el dato a la lista
        byte dato[8];

        for(byte i=0;i<8;i++){
            //Voy a generar un bucle para los primero 8 nibles (32bits)
            if(TAIL == HEAD) 
                dato[i] = 0;        //si tail == head es porque se me agoto el buffer
            else
            {   
                TAIL--; 

                dato[i] = 0;
                dato[i] |= ((*TAIL)-'0');

                
                
                if(TAIL != HEAD){
                    TAIL--;
                    dato[i] |= (((*TAIL)-'0')<<4); 
                }                
            }
        }

        //Una vez que cargue mi buffer auxiliar paso los datos a la tabla
        table_offset &= 0x0FFF;     //Enmascaro el offset ya que si se va mas alla de los 4k explota por fallo de pagina..
    
        for(byte i=0;i<8;i++){ 
            //A la direccion base CHAR_TABLE le sumo table_offset que tiene el desplazamiento en bytes desde el inicio de la tabla
            //Esta direccion la casteo como un char y acceso al contenido de esta posicion de memoria y cargo el valor dato que tengo pre-calculado
            *(char*)(CHAR_TABLE+table_offset) = dato[i];
            //Luego avanzo el puntero de indice
            table_offset++; 
        }

        
        //Si todo salio bien, TAIL deberia apuntar a HEAD, pero sino, fuerzo la situacion
        TAIL = buffer_circular;
        return;
    }

    *TAIL = character;

    TAIL++;

    return;
}