# sendkey
key event generator

## How to build
HOST # export NDK_PATH=/here/is/ndk-build/path
HOST # APP_NAME=[sendkey|inspector|eventconsole] make clean all install

## How to use
HOST # adb push inspector /data/
HOST # adb push sendkey /data/
TARGET # cd /data
TARGET # ./inspector /dev/input/eventN
TARGET # ./sendkey
