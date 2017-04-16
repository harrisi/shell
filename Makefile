# TODO: Find a way to add cross-platform support, ideally without using CMake.
CXX=x86_64-pc-linux-gnu-g++
# TODO: Figure out why this doesn't expand in the compilation step.
CXF=-std=c++1z -Wall -Werror -pedantic

SRC=$(wildcard *.cc)
OBJ=${SRC:.cc=.o}
BIN=shell

.phony: clean

${BIN}: ${OBJ}	
	${CXX} -o $@ $^

%.o: %.c
	${CXX} ${CXF} -c -o $@ $<

clean:
	rm ${OBJ} ${BIN}
