BIN=chip8-emulator
CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++11 -pedantic -Iinclude/
SRC=src/keyboard.cc src/ram.cc src/cpu.cc src/display.cc src/emulator.cc \
	src/disass.cc src/debug.cc src/instruction.cc src/interp.cc src/main.cc
LDFLAGS=`sdl-config --libs`
OBJ=$(SRC:.cc=.o)
TAR=yrakcaz-$(BIN)
PREFIX=/usr/local

-include makefile.rules

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	$(RM) $(OBJ) $(BIN) $(TAR).tar.bz2

distclean: clean
	$(RM) makefile.rules

export:
	git archive HEAD --prefix=$(TAR)/ | bzip2 > $(TAR).tar.bz2

install:
	cp $(BIN) $(PREFIX)/bin

uninstall:
	$(RM) $(PREFIX)/bin/$(BIN)

.PHONY: all clean distclean export install uninstall
