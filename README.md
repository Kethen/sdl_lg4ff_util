## Companion dev tool for work on https://github.com/Kethen/SDL/tree/SDL2_lg4ff

### Features
- runtime loading of sdl2
- dump input from the SDL_Joystick interface from devices of interest
- change wheel compat mode (currently through SDL_JoystickSendEffect)
- set wheel LED (currently through SDL_SDL_JoystickSetLED)
- send test ffb through SDL_Haptic

### Build (Linux/MacOS/FreeBSD)
run build.sh in project root, make sure `gcc` and `g++` are available

### Usage
In a terminal, place libSDL2-2.0.so.0 or libSDL2-2.0.0.dylib at current directory (./), then run build/sdl_lg4ff_util from a terminal
