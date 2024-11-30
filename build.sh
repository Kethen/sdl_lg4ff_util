#!/bin/bash

set -xe

BUILD_DIR="build"

rm -rf $BUILD_DIR
mkdir $BUILD_DIR

CFLAGS="-std=c11 -g -O0"
CPPFLAGS="-std=c++20 -g -O0"

gcc logging.c $CFLAGS -c -o $BUILD_DIR/logging.o
gcc sdl_binder.c $CFLAGS -c -o $BUILD_DIR/sdl_binder.o
g++ device_ids.cpp $CPPFLAGS -c -o $BUILD_DIR/device_ids.o
g++ sdl_joystick.cpp $CPPFLAGS -c -o $BUILD_DIR/sdl_joystick.o
g++ event_loop.cpp $CPPFLAGS -c -o $BUILD_DIR/event_loop.o
g++ mode_change.cpp $CPPFLAGS -c -o $BUILD_DIR/mode_change.o
g++ menu.cpp $CPPFLAGS -c -o $BUILD_DIR/menu.o
g++ main.cpp $CPPFLAGS -c -o $BUILD_DIR/main.o

g++ -o $BUILD_DIR/sdl_lg4ff_util $BUILD_DIR/logging.o $BUILD_DIR/sdl_binder.o $BUILD_DIR/device_ids.o $BUILD_DIR/sdl_joystick.o $BUILD_DIR/event_loop.o $BUILD_DIR/mode_change.o $BUILD_DIR/menu.o $BUILD_DIR/main.o
