.PHONY : clean


OUT=./bin/bios.bin
OBJECTS  = ./obj/functions_c.elf ./obj/init.elf ./obj/main.elf ./obj/sys_tables_asm.elf ./obj/int_handlers.elf ./obj/interrupts.elf ./obj/sys_tables_c.elf ./obj/driver_vga.elf ./obj/vgainit.elf ./obj/kbrd.elf
OBJECTS += ./obj/tarea_1.elf ./obj/tarea_2.elf ./obj/tarea_2_s.elf ./obj/tarea_3.elf ./obj/tarea_3_s.elf ./obj/tarea_4.elf ./obj/paging.elf ./obj/scheduler.elf ./obj/int_handlers_s.elf

ENTRY_POINT=reset./obj/tareas.elf
MAP_FILE=bios.map
PAGE_SIZE=0x1000

AC = nasm
CC = gcc
AFLAGS = -f elf32 -Wall -w-gnu-elf-extensions
CFLAGS = -c -m32 -fno-stack-protector -fno-asynchronous-unwind-tables -Wall -fno-PIC -Wno-pointer-to-int-cast
LDFLAGS = -z max-page-size=$(PAGE_SIZE) --oformat=binary -m elf_i386
LDSCRIPT = linker.ld



all: $(OUT)

$(OUT): $(OBJECTS) $(LDSCRIPT)
	@mkdir -p bin
	ld $(LDFLAGS) -T $(LDSCRIPT) $(OBJECTS) -o $@ -Map $(MAP_FILE)
	@echo "Compilacion Correcta."


./obj/main.elf: ./src/main.s Makefile
	@echo "Compilando main.s..."
	@mkdir -p lst
	@mkdir -p obj
	$(AC) $(AFLAGS) $< -o $@ -l ./lst/main.lst

./obj/init.elf: ./src/init.s Makefile 
	@echo "Compilando init.s..."
	@mkdir -p lst
	@mkdir -p obj
	$(AC) $(AFLAGS) $< -o $@ -l ./lst/init.lst

./obj/sys_tables_asm.elf : ./src/sys_tables.s Makefile 
	@echo "Compilando sys_tables.s..."
	@mkdir -p lst
	@mkdir -p obj
	$(AC) $(AFLAGS) $< -o $@ -l ./lst/sys_tables.lst

./obj/functions_c.elf: ./src/functions.c Makefile
	@echo "Compilando functions.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/int_handlers.elf: ./src/int_handlers.c Makefile
	@echo "Compilando int_handlers.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/interrupts.elf: ./src/interrupts.c Makefile
	@echo "Compilando interrupts.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/sys_tables_c.elf: ./src/sys_tables.c Makefile
	@echo "Compilando sys_tables.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/driver_vga.elf: ./src/driver_vga.c Makefile
	@echo "Compilando driver_vga.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/vgainit.elf: ./src/vgainit.s Makefile
	@echo "Compilando vgainit.s..."
	@mkdir -p lst
	@mkdir -p obj
	$(AC) $(AFLAGS) $< -o $@ -l ./lst/vgainit.lst

./obj/kbrd.elf: ./src/kbrd.c Makefile
	@echo "Compilando kbrd.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/tarea_1.elf: ./src/tarea_1.c Makefile
	@echo "Compilando tarea_1.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/tarea_2.elf: ./src/tarea_2.c Makefile
	@echo "Compilando tarea_2.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/tarea_2_s.elf: ./src/tarea_2.s Makefile
	@echo "Compilando tarea_2.s..."
	@mkdir -p lst
	@mkdir -p obj
	$(AC) $(AFLAGS) $< -o $@ -l ./lst/tarea_2_s.lst

./obj/tarea_3.elf: ./src/tarea_3.c Makefile
	@echo "Compilando tarea_3.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/tarea_3_s.elf: ./src/tarea_3.s Makefile
	@echo "Compilando tarea_3.s..."
	@mkdir -p lst
	@mkdir -p obj
	$(AC) $(AFLAGS) $< -o $@ -l ./lst/tarea_3_s.lst

./obj/tarea_4.elf: ./src/tarea_4.c Makefile
	@echo "Compilando tarea_4.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/paging.elf: ./src/paging.c Makefile
	@echo "Compilando paging.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

./obj/scheduler.elf: ./src/scheduler.c Makefile
	@echo "Compilando scheduler.c..."
	@mkdir -p lst
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@
	

./obj/int_handlers_s.elf: ./src/int_handlers.s Makefile
	@echo "Compilando int_handlers.s..."
	@mkdir -p lst
	@mkdir -p obj
	$(AC) $(AFLAGS) $< -o $@ -l ./lst/int_handlers_s.lst

clean: Makefile
	@echo "Limpiando...."
	rm -rf ./bin/$(OUT) ./lst/*.lst $(OBJECTS) $(MAP_FILE)
	rm -rf ./bin/ ./obj/ ./lst/

bochs: $(OUT)
	bochs -q -f bochs.cfg