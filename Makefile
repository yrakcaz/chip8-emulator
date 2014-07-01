EXE=chip8-emulator
CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -std=c++11 -pedantic
SRC=src/keyboard.cc src/emulator.cc src/main.cc
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
	rm -f makefile.rules

export:
	git archive HEAD --prefix=$(TAR)/ | bzip2 > $(TAR).tar.bz2

.PHONY: all clean distclean export
