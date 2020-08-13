#
#
#  Copyright 2018-2020 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.
#
#

NDK_PATH?=/Users/os.kwon/Library/Android/sdk/ndk/21.0.6113669

APP_NAME?=sendkey
APP_VERSION:=2.1

include jni/Application.mk

all:
	@ export PATH="$(NDK_PATH):${PATH}" && APPNAME=$(APP_NAME) VERSION=$(APP_VERSION) ndk-build -j8

clean:
	@ rm -Rf libs obj bin

install:
	@ mkdir -p bin; cp -avf libs/$(INSTALL_APP_ABI)/$(APP_NAME) ./bin/
