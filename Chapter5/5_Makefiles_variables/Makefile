
CC = arm-none-eabi-gcc
CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11
LDFLAGS = -nostdlib -T stm32_ls.ld -Wl,-Map=5_makefile_project_v2.map

final : 5_makefile_project_v2.elf 

main.o : main.c
	$(CC) $(CFLAGS) $^ -o $@
	
stm32f411_startup.o : stm32f411_startup.c
	$(CC) $(CFLAGS) $^ -o $@
	
5_makefile_project_v2.elf : main.o stm32f411_startup.o
	$(CC) $(LDFLAGS) $^ -o $@

load :
	openocd -f board/st_nucleo_f4.cfg
clean:
	del	-f *.o *.elf *.map