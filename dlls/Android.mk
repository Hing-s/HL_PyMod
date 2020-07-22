LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pymod

include $(XASH3D_CONFIG)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a-hard)
LOCAL_MODULE_FILENAME = libpymod_hardfp
endif

LOCAL_CPPFLAGS := $(LOCAL_CFLAGS) -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections \
			-fno-asynchronous-unwind-tables -fno-unwind-tables -std=c++11 -Wno-narrowing -g

LOCAL_LDFLAGS += -Wl,--gc-sections -ggdb -ldl

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/. \
		$(LOCAL_PATH)/.. \
                $(LOCAL_PATH)/../bot \
                $(LOCAL_PATH)/../utils \
                $(LOCAL_PATH)/../common \
                $(LOCAL_PATH)/../dlls \
                $(LOCAL_PATH)/../engine \
                $(LOCAL_PATH)/../pm_shared \
                $(LOCAL_PATH)/../metamod \
                $(LOCAL_PATH)/../dlls/mod \
                $(LOCAL_PATH)/../Python/Include \

LOCAL_SRC_FILES := \
		h_export.cpp \
		dll.cpp \
		linkfunc.cpp \
		util.cpp \
		pymod/py_main.cpp \
		pymod/py_engine.cpp
		
LOCAL_LDLIBS := -llog -ldl ../Python/libpython3.7m.so 

include $(BUILD_SHARED_LIBRARY)
