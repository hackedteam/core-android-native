LOCAL_PATH := $(call my-dir)
CPP_CORE := $(abspath $(call my-dir)/../../CPPCore)
SHARED := ../../shared

include $(CLEAR_VARS)

LOCAL_MODULE    := runner
LOCAL_SRC_FILES := suidext/runner.c $(SHARED)/utils/deobfuscate.c $(SHARED)/utils/log.c utils/ps.c $(SHARED)/utils/utils.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=40 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=40
LOCAL_C_INCLUDES += headers $(SHARED)/headers
include $(BUILD_EXECUTABLE)
