# CHIP8 Emulator

## About project
This is a CHIP8 emulator written in c++ using SDL2. Written to learn more about low-level stuff after my computer architecture classes.
Tested with few roms, all seem to be working, also testing roms pass with no problem.

## Installation & usage

Install sdl2-dev
```
sudo apt-get install libsdl2-dev
```
then 
```
cmake .
```
It should build the program. If any errors occur there is also a compiled binary included since i had some problems with the compilation process.

Then lauch the emulator, for example with the provided rom located in the binary folder
```
./CHIP8 <rom filename>
```
The provided rom is a breakout game implementation, steer the paddle using q and e


