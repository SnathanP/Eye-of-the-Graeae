# Makefile

CC = gcc
CPPFLAGS = -MMD `pkg-config --cflags --libs gtk+-3.0`
CFLAGS = -c -Wall -Werror -std=c99 -O0 -g 
LDFLAGS =
LDLIBS= -lm `pkg-config --cflags --libs gtk+-3.0`


OBJ = graphical.o main.o
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
