EXE=chip8-emulator
CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++11 -pedantic
SRC=src/keyboard.cc src/RAM.cc src/CPU.cc src/display.cc src/emulator.cc src/main.cc
LDFLAGS=-lSDL
OBJ=$(SRC:.cc=.o)
TAR=yrakcaz-chip8-emulator

-include makefile.rules

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(EXE) $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ) $(EXE) $(TAR).tar.bz2

distclean: clean
	rm -f makefile.rules core

export:
	git archive HEAD --prefix=$(TAR)/ | bzip2 > $(TAR).tar.bz2

boot: all
	./chip8-emulator tests/PONG

.PHONY: all clean distclean export
