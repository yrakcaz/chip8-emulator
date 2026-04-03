[![CI](https://github.com/yrakcaz/chip8-emulator/actions/workflows/ci.yml/badge.svg)](https://github.com/yrakcaz/chip8-emulator/actions/workflows/ci.yml)
[![MIT License](https://img.shields.io/github/license/yrakcaz/chip8-emulator?color=blue)](./LICENSE)

# chip8-emulator

A CHIP-8 virtual machine emulator written in C++11. Supports standard ROM execution, step-by-step debugging, disassembly, and assembly interpretation. Includes 23 classic CHIP-8 ROMs.

## Features

- Full CHIP-8 CPU emulation (16 registers, 4096 bytes RAM, 16-level stack, delay and sound timers)
- SDL-based 64×32 display scaled 10×
- Keyboard input mapped to the CHIP-8 hexadecimal keypad
- Disassembler: convert binary ROMs to readable assembly
- Debugger: step-by-step execution with CPU state display
- Assembler interpreter: convert CHIP-8 assembly back to binary

## Requirements

- SDL 1.2 (`libsdl1.2-dev`)
- g++ or clang++ with C++11 support

On Debian/Ubuntu:

```
apt install libsdl1.2-dev
```

## Build

```
./configure && make
```

Options:

- `--with-debug` — enable debug symbols (`-g3`)
- `--with-clang` — compile with clang++ instead of g++
- `--prefix=DIR` — set install directory (default: `/usr/local`)

## Install

```
make install
```

Installs `chip8-emulator` to `/usr/local/bin` by default. Override with `--prefix`:

```
./configure --prefix=/usr && make install
```

To uninstall:

```
make uninstall
```

## Usage

```
chip8-emulator [option] <file>
```

| Option | Description |
|---|---|
| *(none)* | Run ROM in emulator |
| `-d`, `--disass` | Disassemble ROM to stdout |
| `-g`, `--debug` | Run ROM step by step |
| `-i`, `--interpret` | Interpret a CHIP-8 assembly file |
| `-h`, `--help` | Show help |

## Examples

Run a ROM:

```
chip8-emulator examples/PONG
```

Disassemble a ROM:

```
chip8-emulator --disass examples/TETRIS
```

Debug step by step:

```
chip8-emulator -g examples/INVADERS
```

23 classic CHIP-8 ROMs are included in `examples/`.
