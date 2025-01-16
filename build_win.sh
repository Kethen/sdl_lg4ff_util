#!/bin/bash

set -xe

BUILD_DIR="build_win"

rm -rf $BUILD_DIR
mkdir $BUILD_DIR

CFLAGS="-std=c11 -g -O0"
CPPFLAGS="-std=c++20 -g -O0"

CC=x86_64-w64-mingw32-gcc
CPPC=x86_64-w64-mingw32-g++

$CC logging.c $CFLAGS -c -o $BUILD_DIR/logging.o
$CC sdl_binder.c $CFLAGS -c -o $BUILD_DIR/sdl_binder.o
$CPPC device_ids.cpp $CPPFLAGS -c -o $BUILD_DIR/device_ids.o
$CPPC sdl_joystick.cpp $CPPFLAGS -c -o $BUILD_DIR/sdl_joystick.o
$CPPC event_loop.cpp $CPPFLAGS -c -o $BUILD_DIR/event_loop.o
$CPPC mode_change.cpp $CPPFLAGS -c -o $BUILD_DIR/mode_change.o
$CPPC menu.cpp $CPPFLAGS -c -o $BUILD_DIR/menu.o
$CPPC main.cpp $CPPFLAGS -c -o $BUILD_DIR/main.o

$CPPC -lntdll -Wl,-Bstatic -lpthread -static-libgcc -static-libstdc++ -o $BUILD_DIR/sdl_lg4ff_util.exe $BUILD_DIR/logging.o $BUILD_DIR/sdl_binder.o $BUILD_DIR/device_ids.o $BUILD_DIR/sdl_joystick.o $BUILD_DIR/event_loop.o $BUILD_DIR/mode_change.o $BUILD_DIR/menu.o $BUILD_DIR/main.o
