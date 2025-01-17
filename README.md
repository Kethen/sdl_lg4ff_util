## Companion dev tool for work on https://github.com/Kethen/SDL/tree/SDL2_lg4ff

### Features
- dynamic loading of sdl2
- dump input from the SDL_Joystick interface from devices of interest
- change wheel compat mode (currently through SDL_JoystickSendEffect)
- set wheel LED (currently through SDL_JoystickSetLED)
- send test ffb through SDL_Haptic

### Build (Linux/MacOS/FreeBSD)
run `bash build.sh` in project root, make sure `gcc` and `g++` are available, on MacOS that would mean having some kind of Xcode installed and configured

### Building SDL

```
# clone the project
git clone https://github.com/Kethen/SDL -b SDL2_lg4ff
cd SDL
mkdir sdl_build
cd sdl_build

# configure project (Linux)
../configure --enable-hidapi=yes --enable-hidapi-libusb=yes

# configure project (FreeBSD)
../configure --enable-hidapi=yes --enable-hidapi-libusb=yes

# configure project (MacOS)
../configure --enable-hidapi=yes

# build project (Linux)
make -j $(nproc) # make sure gnu make and gcc are installed

# build project (FreeBSD)
gmake -j $(sysctl -n hw.ncpu) # make sure gnu make and gcc are installed

# build project (MacOS)
make -j $(sysctl -n hw.ncpu) # make sure some kind of Xcode is installed and configured, alternatively the Xcode project file should also work

```

### Preparing device node permission

#### Linux

Place udev rule from https://raw.githubusercontent.com/Kethen/lg4ff_userspace/refs/heads/main/60-lg4ff-userspace.rules to `/etc/udev/rules.d`, then run `sudo udevadm control --reload; sudo udevadm trigger`, that will flag supported wheels' /dev/hidraw* nodes as user accessible

#### FreeBSD

It's weird, it needs both hidraw device AND raw usb access to work, and it looks like neither `devfs.conf` nor `devfs.rules` can accurately only make supported device accessible. For now just go for `sudo chmod 666 /dev/usb/*; sudo chmod /dev/hidraw*`. Perhaps a service for tagging just supported device is needed, because flagging every usb device raw accessible has security implications.

For `/dev/hidraw*` to show up at all, one would also need to add `hw.usb.usbhid.enable=1` to /etc/sysctl.conf, as well as adding `usbhid_load="YES"` and `hidraw_load="YES"` to `/boot/loader.conf`

#### MacOS

It does not look like one need to do anything to allow IOHidManager access to wheels, it should just work, at least on intel Macs

### Usage
In a terminal, place `libSDL2-2.0.so.0` or `libSDL2-2.0.0.dylib` in the current directory (./), then run `build/sdl_lg4ff_util`; you will be greeted with a text based UI

```
# Linux/FreeBSD:
cat <sdl build directory>/build/.libs/libSDL2-2.0.so.0 > libSDL2-2.0.so.0
./build/sdl_lg4ff_util
```

```
# MacOS make:
cat <sdl build directory>/build/.libs/libSDL2-2.0.0.dylib > libSDL2-2.0.0.dylib
./build/sdl_lg4ff_util
```

```
# MacOS Xcode:
cat ~/Library/Developer/Xcode/DerivedData/SDL-<?>/Build/Products/Debug/libSDL2.dylib > libSDL2-2.0.0.dylib
./build/sdl_lg4ff_util
```

sdl joystick events are logged in `./sdl_event_log.txt`

### Testing with WINE

#### Linux/FreeBSD using LD_LIBRARY_PATH

```
# close all wine first and wait until all wineserver instances are gone
wineserver -w
# run wine with the library and confirm that the wheel is loaded in control panel
LD_LIBRARY_PATH="$(realpath .)" wine control
# run wine ffb applications
LD_LIBRARY_PATH="$(realpath .)" wine start <path to application>
```

#### MacOS with CrossOver

1. Fully shutdown CrossOver
2. Go to your crossover .app directory (Show package content)
3. Navigate to Contents/SharedSupport/CrossOver/lib64
4. Rename libSDL2-2.0.0.dylib to something else
5. Place your built libSDL2-2.0.0.dylib into the directory
6. Check Game Controllers tab in a bottle to see if the wheel is loaded
7. Launch FFB applications in CrossOver

#### MacOS with Kegworks keg instances

1. Fully shutdown the keg instance
2. Go to your keg directory (~/Applications/Kegworks/) and navigate into the .app directory (Show package content)
3. Navigate to Contents/Frameworks
4. Rename libSDL2-2.0.0.dylib to something else
5. Place your built libSDL2-2.0.0.dylib into the directory
6. Launch control panel in the keg to see if the wheel is loaded in game controller settings
7. Launch FFB applications in the keg
