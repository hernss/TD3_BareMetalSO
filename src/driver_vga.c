#include "../inc/prototipos.h"

//https://wiki.osdev.org/Printing_To_Screen


#define ROWS 25
#define COLS 80

#define MATRIX_LENGTH (ROWS*COLS)


#define USE_COLOR 1

#ifdef USE_COLOR
    #define BASE_VGA 0x10000
#else
    #define BASE_VGA 0xB0000
#endif

#define BLINK   0x80
#define INV     0x70
#define BOLD    0x04
#define NORMAL  0x03
#define BLANK_ATTR 0x1F

#define FIRST_BAR 23*COLS+1
#define LAST_BAR FIRST_BAR+78

#pragma pack(2)
typedef struct{
    char character;
    byte attribute;
} item_vga;

extern word table_offset;
extern qword promedio;
extern qword suma;
extern qword suma_sw;
extern qword suma_sq;

__attribute__(( section(".kernel_bss"))) word progress_bar_offset = 0;

__attribute__(( section(".functions_ram"))) void mostrar_suma_sq(){
    item_vga* vram_map = (item_vga*)BASE_VGA; 
    const char ascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    const char sum[] = "Suma SQ: 0x";

    char* dato = (char*)&suma_sq;
    char *p = (char*)sum;
    word cursor = 11*COLS + COLS-29;

    while(*p != '\0'){
        vram_map[cursor].character = *p;
        vram_map[cursor++].attribute = BLANK_ATTR;
        p++;
    }

    for(byte i=0;i<8;i++){
        vram_map[cursor + (14-2*i)].character = ascii[(((*dato)&0xF0)>>4)];
        vram_map[cursor + (14-2*i)].attribute = BLANK_ATTR;

        vram_map[cursor + (15-2*i)].character = ascii[((*dato)&0x0F)];
        vram_map[cursor + (15-2*i)].attribute = BLANK_ATTR;
        dato++;
    }

    return;
}

__attribute__(( section(".functions_ram"))) void mostrar_suma_sw(){
    item_vga* vram_map = (item_vga*)BASE_VGA; 
    const char ascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    const char sum[] = "Suma SW: 0x";

    char* dato = (char*)&suma_sw;
    char *p = (char*)sum;
    word cursor = 10*COLS + COLS-29;

    while(*p != '\0'){
        vram_map[cursor].character = *p;
        vram_map[cursor++].attribute = BLANK_ATTR;
        p++;
    }

    for(byte i=0;i<8;i++){
        vram_map[cursor + (14-2*i)].character = ascii[(((*dato)&0xF0)>>4)];
        vram_map[cursor + (14-2*i)].attribute = BLANK_ATTR;

        vram_map[cursor + (15-2*i)].character = ascii[((*dato)&0x0F)];
        vram_map[cursor + (15-2*i)].attribute = BLANK_ATTR;
        dato++;
    }

    return;
}

__attribute__(( section(".functions_ram"))) void mostrar_suma(){
    item_vga* vram_map = (item_vga*)BASE_VGA; 
    const char ascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    const char sum[] = "Suma: 0x";

    char* dato = (char*)&suma;
    char *p = (char*)sum;
    word cursor = 10*COLS + COLS-26;

    while(*p != '\0'){
        vram_map[cursor].character = *p;
        vram_map[cursor++].attribute = BLANK_ATTR;
        p++;
    }

    for(byte i=0;i<8;i++){
        vram_map[cursor + (14-2*i)].character = ascii[(((*dato)&0xF0)>>4)];
        vram_map[cursor + (14-2*i)].attribute = BLANK_ATTR;

        vram_map[cursor + (15-2*i)].character = ascii[((*dato)&0x0F)];
        vram_map[cursor + (15-2*i)].attribute = BLANK_ATTR;
        dato++;
    }

    return;
}

__attribute__(( section(".functions_ram"))) void mostrar_promedio(){
    item_vga* vram_map = (item_vga*)BASE_VGA; 

    word cursor = 9*COLS + COLS-30;

    char ascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char prom[] = "Promedio: 0x";

    char* dato = (char*)&promedio;
    char* p = prom;

    while(*p != '\0'){
        vram_map[cursor].character = *p;
        vram_map[cursor++].attribute = BLANK_ATTR;
        p++;
    }
 
    for(byte i=0;i<8;i++){
        vram_map[cursor + (14-2*i)].character = ascii[(((*dato)&0xF0)>>4)];
        vram_map[cursor + (14-2*i)].attribute = BLANK_ATTR;

        vram_map[cursor + (15-2*i)].character = ascii[((*dato)&0x0F)];
        vram_map[cursor + (15-2*i)].attribute = BLANK_ATTR;
        dato++;
    }

}

__attribute__(( section(".functions_ram"))) void check_list(){
    static word last_update = 0;

    //Todavia no vino ningun item o ya lo actualice
    if(last_update == table_offset) return;

    char* tabla = (char*)CHAR_TABLE;

    tabla += table_offset-8;       //Divido le resto 8 para entrar a la ultima posicion que cargue en la tabla

    __print_list_item(tabla);
    last_update = table_offset;
}

__attribute__(( section(".functions_ram"))) void __print_list_item(char* dato){
    item_vga* vram_map = (item_vga*)BASE_VGA;
    static word row_offset = 0;

    const char ascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    if(row_offset == 7){
        //debo hacer el scroll
        row_offset--;
        for(byte r=3;r<9;r++){
            for(byte c=41;c<79;c++){
                word ofs = r*COLS+c;
                vram_map[ofs-COLS].character = vram_map[ofs].character;
                vram_map[ofs-COLS].attribute = vram_map[ofs].attribute;
                if(r==8){
                    vram_map[ofs].character = ' ';
                }
            }
        }
    }

    word cursor = 2*COLS + row_offset*COLS + COLS-20;

    vram_map[cursor].character = '0';
    vram_map[cursor++].attribute = BLANK_ATTR;

    vram_map[cursor].character = 'x';
    vram_map[cursor++].attribute = BLANK_ATTR;

    
    for(byte i=0;i<8;i++){
        vram_map[cursor + (14-2*i)].character = ascii[(((*dato)&0xF0)>>4)];
        vram_map[cursor + (14-2*i)].attribute = BLANK_ATTR;

        vram_map[cursor + (15-2*i)].character = ascii[((*dato)&0x0F)];
        vram_map[cursor + (15-2*i)].attribute = BLANK_ATTR;
        dato++;
    }
    row_offset++;
}
__attribute__(( section(".functions_ram"))) void add_progress(){
    item_vga* vram_map = (item_vga*)BASE_VGA;

    if(!progress_bar_offset){
        progress_bar_offset = FIRST_BAR; //Como no puedo inicializarlo en un valor distinto de 0 entonces lo hago la primera vez que entro a la funcion
    } else {
        progress_bar_offset++;
    }

    if(progress_bar_offset==LAST_BAR){
        progress_bar_offset = FIRST_BAR;

        for(word i=FIRST_BAR;i<LAST_BAR; i++){          //Pongo en blanco el progress bar
            vram_map[i].character = ' ';
            vram_map[i].attribute = BLANK_ATTR;
        }
    }

    vram_map[progress_bar_offset].character = '-';
    vram_map[progress_bar_offset].attribute = BLANK_ATTR;
}
__attribute__(( section(".functions_ram"))) void print(char character){
    item_vga* vram_map = (item_vga*)BASE_VGA;
    static word cursor_offset = 0;

    if(character == 0) return;

    if(cursor_offset == 0){
        cursor_offset = 161;
    }else{
        cursor_offset++;
    }
    
    if(character == '\n'){
        //Tengo que ingresar una nueva linea.
        while(cursor_offset%COLS) cursor_offset++;      //Incremento el cursor hasta que sea multiplo de 80
        if(cursor_offset >= 9*COLS){                   
            //Llegue al final de la pantalla
            cursor_offset -= COLS;                      //Pongo el cursor en la linea anterior
            __scroll_input();                                   //Corro toda la pantalla para arriba
        }
        return;
    }

    
    if(!((cursor_offset-40)%COLS)){
        while(cursor_offset%COLS) cursor_offset++;      //Incremento el cursor hasta que sea multiplo de 80
        cursor_offset++;       
    }

    if(cursor_offset >= 9*COLS){                   
        //Llegue al final de la pantalla
        cursor_offset -= COLS;                      //Pongo el cursor en la linea anterior
        __scroll_input();                                   //Corro toda la pantalla para arriba
    }

    vram_map[cursor_offset].character = character;
    vram_map[cursor_offset].attribute = BLANK_ATTR;
}

__attribute__(( section(".functions_ram"))) void printXY(word x, word y, char character){
    item_vga* vram_map = (item_vga*)BASE_VGA;

    word offset = x + y * COLS;

    vram_map[offset].character = character;
    vram_map[offset].attribute = BLANK_ATTR;
}

__attribute__(( section(".functions_ram"))) void scroll(){
    item_vga* vram_map = (item_vga*)BASE_VGA;

    for(word i=COLS;i<MATRIX_LENGTH; i++){          //i Arranca en el primer caracter de la 2da fila
        vram_map[i-COLS] = vram_map[i];             // Desplazo todos los campos COLS bytes hacia abajo, por ente lo paso a la fila anterior
    }
    for(word i=(ROWS-1)*COLS;i<MATRIX_LENGTH; i++){          //Pongo en blanco la ultima fila
        vram_map[i].character = ' ';
        vram_map[i].attribute = BLANK_ATTR;
    }
}

__attribute__(( section(".functions_ram"))) void __scroll_log(){
    item_vga* vram_map = (item_vga*)BASE_VGA;

    for(word i=11*COLS;i<21*COLS; i++){          //i Arranca en el primer caracter de la 2da fila
        vram_map[i-COLS] = vram_map[i];             // Desplazo todos los campos COLS bytes hacia abajo, por ente lo paso a la fila anterior
    }
    for(word i=20*COLS+1;i<21*COLS-1; i++){          //Pongo en blanco la ultima fila
        vram_map[i].character = ' ';
        vram_map[i].attribute = BLANK_ATTR;
    }
}

__attribute__(( section(".functions_ram"))) void __scroll_input(){
    item_vga* vram_map = (item_vga*)BASE_VGA;
    for(byte c=1;c<40;c++){
    for(word r=3;r<9; r++){          
        vram_map[(r-1)*COLS+c] = vram_map[r*COLS+c];             // Desplazo todos los campos COLS bytes hacia abajo, por ente lo paso a la fila anterior
    }
    }
    for(word i=8*COLS+1;i<8*COLS+40; i++){          //Pongo en blanco la ultima fila
        vram_map[i].character = ' ';
        vram_map[i].attribute = BLANK_ATTR;
    }
}

__attribute__(( section(".functions_ram"))) void clear(){
    item_vga* vram_map = (item_vga*)BASE_VGA;

    char pit_str[14] = "PIT Progress:";
    char log[8] = " DEBUG:";
    char inp[8] = " INPUT:";
    char titulo[42] = "Alumno: Hernan M. Travado  Leg: 131.453-1";

    log[0] = 205;
    inp[0] = 205;

    //Borro toda la pantalla
    for(word i=0;i<MATRIX_LENGTH; i++){
        vram_map[i].character = ' ';
        vram_map[i].attribute = BLANK_ATTR;
    }

    //Dibujo las divisiones de los campos a mostrar
    for(word i=80;i<MATRIX_LENGTH; i++){
        
        if((i>80)&&(i<80+79)){              //Primera fila.. todas con barra horizontal
            vram_map[i].character = 205;    //Barra Horizontal
        }else if(!(i%COLS)){                //i%80 refiere a la columna del margen izquierdo
            vram_map[i].character = 186;    //Barra Vertical
        }else if(!((i+1)%COLS)){            // (i+1)%80 refiere a la columna del margen derecho
            vram_map[i].character = 186;    //Barra Vertical
        }
        if(i>(ROWS-1)*COLS){                //Ultima fila todas barras horizontales
            vram_map[i].character = 205;    //Barra Horizontal
        }

        if((i>21*COLS)&&(i<22*COLS)){       //Separacion del PIT
            vram_map[i].character = 205;    //Barra Horizontal
        }

        if((i>9*COLS)&&(i<10*COLS)){       //Separacion entre las listas y el log
            vram_map[i].character = 205;    //Barra Horizontal
        }

        if(i<8*COLS+41){
            if(!((i-40)%COLS)){
                vram_map[i].character = 186;    //Barra Vertical
            }
        }
    }

    //Ahora agrego las esquinas y las intersecciones1
    vram_map[120].character = 203;    //TEE SUP
    vram_map[9*COLS+40].character = 202;    //TEE INF
    vram_map[COLS].character = 201;    //Esq sup Izq
    vram_map[2*COLS-1].character = 187;    //Esq sup der
    vram_map[24*COLS].character = 200;    //Esq Inf Izq
    vram_map[25*COLS-1].character = 188;    //Esq inf Der
    vram_map[9*COLS].character = 204;    //TEE der
    vram_map[10*COLS-1].character = 185;    //TEE izq
    vram_map[21*COLS].character = 204;    //TEE der
    vram_map[22*COLS-1].character = 185;    //TEE izq

    //Escribo los titulos
    __printline(inp, 1);
    __printline(log, 9);
    __printline(titulo, 0);
    __printline(pit_str, 22);

    printXY(42,1,'L');
    printXY(43,1,'I');
    printXY(44,1,'S');
    printXY(45,1,'T');
    printXY(46,1,'A');
    printXY(47,1,':');
}

__attribute__(( section(".functions_ram"))) void __printline(char* str,byte nro_linea){
    item_vga* vram_map = (item_vga*)BASE_VGA;

    if(nro_linea>25) return;

    word n = nro_linea*COLS + 1;

    while(*str != '\0'){
        vram_map[n++].character = *str++; 
    }
}

__attribute__(( section(".functions_ram"))) void printlog(const char* _str){
    static byte log_line = 0;

    char* str = (char*)_str;

    if(log_line > 10){
        log_line = 10;
        __scroll_log();
    }

    __printline(str, log_line+10);    

    log_line++;
}
