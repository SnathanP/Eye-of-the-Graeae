## Simple SDL mini code

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -g
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

OBJ= pixel_operations.o picture_treatment.o main.o
DEP= ${SRC:.o=.d}

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} *~
	${RM} main
	${RM} *.d

-include ${DEP}

# END
