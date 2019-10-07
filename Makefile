APP_NAME?=sendkey
NDK_PATH?=/Users/oskwon/Desktop/Alticast/android-ndk-r18b
APP_VERSION?=1.5

all:
	@ export PATH="$(NDK_PATH):${PATH}" && APPNAME=$(APP_NAME) VERSION=$(APP_VERSION) ndk-build -j8

clean:
	@ rm -Rf libs obj eventconsole inspector sendkey

install:
	@ cp -avf libs/armeabi-v7a/$(APP_NAME) ./
