LOCAL_PATH := $(call my-dir)
CPP_CORE := $(abspath $(call my-dir)/../../CPPCore)
SHARED := ../../shared

include $(CLEAR_VARS)

LOCAL_MODULE    := bbmdecoder
LOCAL_SRC_FILES := bbmdecoder/bbmdecoder.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -fPIE -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=40 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=40
LOCAL_LDFLAGS += -fPIE -pie
LOCAL_C_INCLUDES += headers $(SHARED)/headers
include $(BUILD_EXECUTABLE)
