# TODO: Find a way to add cross-platform support, ideally without using CMake.
#CXX=x86_64-pc-linux-gnu-g++
CXX=x86_64-w64-mingw32-g++
CXF=-std=c++1z -Wall -Werror -pedantic

SRC=$(wildcard *.cc)
OBJ=${SRC:.cc=.o}
BIN=shell

.phony: clean

${BIN}: ${OBJ}	
	${CXX} -o $@ $^

%.o: %.cc
	${CXX} ${CXF} -c -o $@ $<

clean:
	rm ${OBJ} ${BIN}
