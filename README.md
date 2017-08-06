# chip8c
A semi-functional Chip8 emulator in C.

## Synopsis
A Chip8 emulator in C that doesn't quite work yet. Written on Windows 10.

## Motivation
I wanted to learn some basic SDL so I made this as a practice project.

## Installation
### Compile (for windows):
```
gcc chip8.c test.c display.c main.c -IPath\To\SDL2\Includes -w -Wl,-subsystem,windows -LPath\To\Libs -lmingw32 -lSDL2main -lSDL2 -o chip8c.exe 
```

Linux compilation will be slightly different but shouldn't be hard to figure out.

### Run:
```
.\chip8c.exe path\to\rom
```

## Warnings
* It still doesn't work well enough to use due to some bug(s), presumably in chip8.c
* test.c is very basic and finds only the most obvious errors in the instructions

## Future improvements
* Fix the bug that makes it not work properly
* Add a debugger
* Add more options for controlling the IPS, button mapping, etc
