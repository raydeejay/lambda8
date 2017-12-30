BINARY=lambda8
CC=gcc
CFLAGS=-O3 -Wall -pedantic `sdl2-config --cflags`
LDFLAGS= -lm `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf

.PHONY: clean

all: $(BINARY)

$(BINARY): aria.o api.o gapbuffer.o buffer.o terminal.o main.o
	${CC} ${CFLAGS} $^ ${LDFLAGS} -o ${BINARY}

clean:
	rm -f *.o ${BINARY} nul

# for flymake
check-syntax:
	${CC} -Wall -pedantic ${CFLAGS} -o nul -S ${CHK_SOURCES}

make.depend: aria.c main.c aria.h api.c api.h nanosvg.h nanosvgrast.h gapbuffer.h gapbuffer.c buffer.h buffer.c terminal.h terminal.c
	touch make.depend
	makedepend -I/usr/include/linux -I/usr/lib/gcc/x86_64-linux-gnu/5/include/ -fmake.depend $^

-include make.depend
