#
#
#  Copyright 2018-2019 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.
#
#
APPNAME?=sendkey

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	LinuxEvent.cpp \
	LinuxInput.cpp \
	LinuxInputWriter.cpp \
	ScriptExecutor.cpp \
	Database.cpp \
	Queue.cpp \
	Util.cpp \
	Console.cpp \
	Logger.cpp

LOCAL_SRC_FILES += \
	example/$(APPNAME).cpp

LOCAL_CFLAGS += -std=c++0x -g -fstack-protector-all -fPIC -pthread -DVERSION=$(VERSION)
LOCAL_C_INCLUDES += $(LOCAL_PATH)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := $(APPNAME)

include $(BUILD_EXECUTABLE)
