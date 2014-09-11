LOCAL_PATH := $(call my-dir)
CPP_CORE := $(abspath $(call my-dir)/../../CPPCore)

include $(CLEAR_VARS)

LOCAL_MODULE    := runner
LOCAL_SRC_FILES := suidext/runner.c utils/deobfuscate.c utils/ps.c utils/log.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=40 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=40
LOCAL_C_INCLUDES += headers
include $(BUILD_EXECUTABLE)
