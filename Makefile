#
#
#  Copyright 2018-2019 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.
#
#

NDK_PATH?=/Users/oskwon/Desktop/Alticast/android-ndk-r18b

APP_NAME?=sendkey
APP_VERSION:=1.6

include jni/Application.mk

all:
	@ export PATH="$(NDK_PATH):${PATH}" && APPNAME=$(APP_NAME) VERSION=$(APP_VERSION) ndk-build -j8

clean:
	@ rm -Rf libs obj bin

install:
	@ mkdir -p bin; cp -avf libs/$(INSTALL_APP_ABI)/$(APP_NAME) ./bin/
