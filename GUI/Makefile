# Makefile

CC = gcc
CPPFLAGS = -MMD `pkg-config --cflags sdl2` `pkg-config --cflags --libs gtk+-3.0`
CFLAGS = -c -Wall -Werror -std=c99 -O0 -g 
LDFLAGS =
LDLIBS= `pkg-config --libs sdl2` `pkg-config --libs --libs gtk+-3.0` -lSDL2_image -lm


OBJ = graphical.o main.o
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
