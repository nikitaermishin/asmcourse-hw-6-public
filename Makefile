NASM = nasm

ASMFLAGS += -g -f elf32

CFLAGS ?= -O2 -g

CFLAGS += -std=gnu99

CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
	-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
	-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wabsolute-value

CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

CC += -m32 -no-pie -fno-pie

LDLIBS = -lm

.PHONY: all clean test

all: integral

integral: integral.c functions.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

functions.o: functions.asm
	$(NASM) $(ASMFLAGS) $< -o $@

test: all
	./integral --test-root 4:5:3.0:6.0:0.0001:5.0
	./integral --test-root 4:6:-5.0:-3.0:0.0001:-4.0
	./integral --test-root 5:6:1.0:4.0:0.0001:2.75

	./integral --test-integral 4:-1.0:2.0:0.0001:-9.0
	./integral --test-integral 5:5.0:6.0:0.0001:23.0
	./integral --test-integral 7:0.0:3.0:0.00001:66.6

clean:
	rm -rf *.o