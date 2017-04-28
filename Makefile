# TODO: Find a way to add cross-platform support, ideally without using CMake.
#   See if autotools supports Windows well; if not, consider CMake, though
# all necessary feature tests can likely be done by hand at no great effort.
# TODO: Find a way to select compiler based on platform.
# TODO: Invoke Cygwin compiler on Windows.
# TODO: Detect compiler string.
CXX=gcc
CXF=-std=c11 -Wall -Werror -pedantic

SRC=$(wildcard *.c)
OBJ=${SRC:.c=.o}
BIN=shell

.phony: clean

${BIN}: ${OBJ}	
	${CXX} -o $@ $^

%.o: %.c
	${CXX} ${CXF} -c -o $@ $<

clean:
	rm ${OBJ} ${BIN}
