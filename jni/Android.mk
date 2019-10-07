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
	Console.cpp

LOCAL_SRC_FILES += \
	example/$(APPNAME).cpp

LOCAL_CFLAGS += -std=c++11 -g -fstack-protector-all -fPIC -pthread -DVERSION=$(VERSION)
LOCAL_C_INCLUDES += $(LOCAL_PATH)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := $(APPNAME)

include $(BUILD_EXECUTABLE)