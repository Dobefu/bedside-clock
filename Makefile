SRCS = $(wildcard *.c lib/*.c utils/*.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -O2 -ffreestanding -fno-stack-protector -nostdinc -nostdlib -nostartfiles -mstrict-align

all: clean kernel8.img

start.o: start.S
	aarch64-elf-gcc $(CFLAGS) -c start.S -o start.o

%.o: %.c
	aarch64-elf-gcc $(CFLAGS) -c $< -o $@

font_sfn.o: font.sfn
	aarch64-elf-ld -r -b binary -o font_sfn.o font.sfn

kernel8.img: start.o font_sfn.o $(OBJS)
	aarch64-elf-ld -nostdlib start.o font_sfn.o $(OBJS) -T link.ld -o kernel8.elf
	aarch64-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio
