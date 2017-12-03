# Makefile

CC = gcc
CPPFLAGS = `pkg-config --cflags sdl2` -MMD `pkg-config --cflags --libs gtk+-3.0`
CFLAGS = -Wall -Werror -Wextra -std=c99 -O0 -g
LDFLAGS =
LDLIBS= -lm `pkg-config --libs sdl2` -lSDL2_image `pkg-config --cflags --libs gtk+-3.0`


OBJ = main.o activation.o matrix.o algebra.o LayerStruct.o savesystem.o Picture_Treatment/picture_treatment.o Picture_Treatment/pixel_operations.o GUI/graphical.o
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
