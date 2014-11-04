LOCAL_PATH := $(call my-dir)
CPP_CORE := $(abspath $(call my-dir)/../../CPPCore)

include $(CLEAR_VARS)
LOCAL_MODULE    := checksum
LOCAL_SRC_FILES := remote_main/install_main_lib.c utils/log.c utils/deobfuscate.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -fvisibility=hidden -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=80 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=80
LOCAL_C_INCLUDES += headers
include $(BUILD_SHARED_LIBRARY)
