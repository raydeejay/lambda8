BINARY=lambda8
CC=gcc
CFLAGS=-O2 -g -Wall -pedantic `sdl2-config --cflags`
LDFLAGS=-lm `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf

.PHONY: clean

all: $(BINARY)

$(BINARY): api.o gapbuffer.o buffer.o terminal.o editor.o main.o s7.o
	${CC} ${CFLAGS} $^ ${LDFLAGS} -o ${BINARY}

clean:
	rm -f *.o ${BINARY} nul

# for flymake
check-syntax:
	${CC} -Wall -pedantic ${CFLAGS} -o nul -S ${CHK_SOURCES}

make.depend: main.c api.c api.h gapbuffer.h gapbuffer.c buffer.h buffer.c terminal.h terminal.c s7.c s7.h editor.c editor.h
	touch make.depend
	makedepend -I/usr/include/linux -I/usr/lib/gcc/x86_64-linux-gnu/5/include/ -fmake.depend $^

-include make.depend
