# TODO: Find a way to add cross-platform support, ideally without using CMake.
#   See if autotools supports Windows well; if not, consider CMake, though
# all necessary feature tests can likely be done by hand at no great effort.
# TODO: Find a way to select compiler based on platform.
# TODO: Invoke Cygwin compiler on Windows.
# TODO: Detect compiler string.
CC=gcc
CF=-std=c11 -Wall -Werror -pedantic
LF=-lutf8proc

SRC=$(wildcard *.c)
OBJ=${SRC:.c=.o}
BIN=shell

.phony: clean

${BIN}: ${OBJ}	
	${CC} ${LF} -o $@ $^

%.o: %.c
	${CC} ${CF} -c -o $@ $<

clean:
	rm ${OBJ} ${BIN}
