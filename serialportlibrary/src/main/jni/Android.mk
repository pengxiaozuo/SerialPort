LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := serialport
LOCAL_SRC_FILES := SerialPortJni.cpp \
					serial_port.cpp
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lm -llog
include $(BUILD_SHARED_LIBRARY)