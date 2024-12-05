## Companion dev tool for work on https://github.com/Kethen/SDL/tree/SDL2_lg4ff

### Features
- dynamic loading of sdl2
- dump input from the SDL_Joystick interface from devices of interest
- change wheel compat mode (currently through SDL_JoystickSendEffect)
- set wheel LED (currently through SDL_JoystickSetLED)
- send test ffb through SDL_Haptic

### Build (Linux/MacOS/FreeBSD)
run `bash build.sh` in project root, make sure `gcc` and `g++` are available

### Usage
In a terminal, place `libSDL2-2.0.so.0` or `libSDL2-2.0.0.dylib` in the current directory (./), then run `build/sdl_lg4ff_util`; you will be greeted with a text based UI

```
Linux/FreeBSD:
$ cat <sdl build directory>/build/.libs/libSDL2-2.0.so.0 > libSDL2-2.0.so.0
$ ./build/sdl_lg4ff_util
```

```
MacOS Xcode:
$ cat ~/Library/Developer/Xcode/DerivedData/SDL-<?>/Build/Products/Debug/libSDL2.dylib > libSDL2-2.0.0.dylib
$ ./build/sdl_lg4ff_util
```

```
MacOS make:
$ cat <sdl build directory>/build/.libs/libSDL2-2.0.0.dylib > libSDL2-2.0.0.dylib
$ ./build/sdl_lg4ff_util
```

sdl joystick events are logged in `./sdl_event_log.txt`
