
#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H 1


typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned long long qword;

#define ERROR_DEFECTO -1
#define EXITO 1

#define CS_SEL_C 0x08
#define DS_SEL_C 0x10
#define TSS_SEL_C 0x018
#define CS_SEL_C_USER 0x020
#define DS_SEL_C_USER 0x028

//Prototipos de funciones del teclado
#define CHAR_TABLE 0x01210000


char get_ascii(byte keycode);
void init_keyboard();
void almacenar_buffer(char character);


//Prototipos de Funciones de VGA
void mostrar_promedio();
void mostrar_suma();
void mostrar_suma_sw();
void mostrar_suma_sq();
void printlog(const char* _str);
void __scroll_log();
void __scroll_input();
void __printline(char* str, byte nro_linea);
void __print_list_item(char* dato);
void check_list();
void print(char character);
void printXY(word x, word y, char character);
void scroll();
void clear();
void add_progress();

//Prototipos de Funciones de I/O
unsigned char inb (unsigned short int __port);
void outb (unsigned char __value, unsigned int __port);

//Prototipo para crear GDT
byte create_gdt();

//Prototipo para crear IDT
byte create_idt();

//Prototipos de los Handlers de Interrupcion
byte handler_teclado();
byte handler_AC();
byte handler_SS();
byte handler_UD();
byte handler_DF();
byte handler_GP();
byte handler_PF();
byte handler_PIT0();
byte handler_INT80();
byte handler_NM();
byte default_interrupt();           //handler por defecto que tienen cargado las entradas de la tabla que no estan definidas


//Prototipos de la tarea 1
void Tarea_promedio();
byte leer_address(byte *addr);

//Prototipos de la tarea 2
void Tarea_suma1();
void sum_sw(qword* dest, qword* src);   //dest = dest + src

//Prototipos de la tarea 3
void Tarea_suma2();
void sum_sq(qword* dest, qword* src);   //dest = dest + src

//Prototipos de la tarea 4
void Tarea_idle();

//Prototipos de la paginacion
#define EXTRACT_TP_OFFSET(la)  ((((dword)la) & 0x003FF000) >> 12)
#define EXTRACT_DTP_OFFSET(la) ((((dword)la) & 0xFFC00000) >> 22)
#define EXTRACT_OFFSET(la)     (((dword)la) & 0x00000FFF)

#define DPT_LINEAL 0x00009000

#define US_USER 1
#define US_SUP 0
#define RW_READ 0
#define RW_WRITE 1

#define ROM_START 0xFFFF0000
#define ROM_PAGE_COUNT 16

dword __create_paging (dword base_address, byte PCD, byte PWT);
dword __create_page_table (dword directory_base_address, word offset, dword page_table_base_address, byte PCD, byte PWT, byte USER_SUPERVISOR, byte READ_WRITE);
void __create_page (dword page_table_base_address, word offset, dword page_base_address, byte PCD, byte PWT, byte GLOBAL, byte USER_SUPERVISOR, byte READ_WRITE);
dword __get_new_page();
dword __get_new_random_page();
dword inicializar_paginacion();
byte test_DTP(dword cr3, word dtp_offset);
byte test_TP(dword dte, word tp_offset);


//Prototipos del scheduler
enum{
    idle =0,
    running,
    stop,
    pending
};

#define CANTIDAD_TAREAS 4

#pragma pack(2)
typedef struct 
{
    word previous_task;     
    word reserved1;
    dword ESP0;
    word SS0;
    word reserved2;
    dword ESP1;
    word SS1;
    word reserved3;
    dword ESP2;
    word SS2;
    word reserved4;
    dword CR3;
    dword EIP;
    dword EFLAGS;
    dword EAX;
    dword ECX;
    dword EDX;
    dword EBX;
    dword ESP;
    dword EBP;
    dword ESI;
    dword EDI;
    word ES;
    word reserved5;
    word CS;
    word reserved6;
    word SS;
    word reserved7;
    word DS;
    word reserved8;
    word FS;
    word reserved9;
    word GS;
    word reserved10;
    word LDT;
    word reserved11;
    word T;
    word IOMap;
} st_trueTSS;

#pragma pack(2)
typedef struct 
{   
    dword eax;      //0
    dword ebx;      //4 
    dword ecx;      //8
    dword edx;      //12

    dword esi;      //16
    dword edi;      //20
    dword ebp;      //24
    
    word  cs;       //28
    word  ds;       //30
    word  es;       //32
    word  fs;       //34
    word  gs;       //36

    word  ss0;      //38
    dword esp0;     //40
    
    word  ss3;      //44
    word  dmy;
    dword esp3;     //48

    dword cr3;      //52
    dword eip;      //56
    dword eflags;   //60
    dword context_id;//64
    dword flag_mmx; //68
} item_faketss;

typedef struct 
{   
    byte status;
    word ss;
    dword stack_pointer;
    word cs;
    dword ip;
    dword cr3;      
    byte pending_running;
    item_faketss *context;

} item_tarea;

void inicializar_scheduler();
void change_context();


//INT 80 OP CODES
enum{
    INT_80_HLT = 1,
    INT_80_TASK_SWITCH,
    INT_80_SHOW_ADD_SW,
    INT_80_SHOW_ADD_SQ,
    INT_80_SHOW_AVG,
    INT_80_PRINT,
    INT_80_READ,
    INT_80_READ_COUNT
};





#endif