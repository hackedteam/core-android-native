LOCAL_PATH := $(call my-dir)
CPP_CORE := $(abspath $(call my-dir)/../../CPPCore)

include $(CLEAR_VARS)
LOCAL_MODULE    := selinux_suidext
LOCAL_SRC_FILES := suidext/su.c suidext/daemon.c suidext/suidext.c utils/deobfuscate.c utils/knox_manager.c utils/xperia_ric_manager.c utils/pts.c utils/utils.c utils/log.c utils/old_shell.c utils/boot_manager.c utils/ps.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=40 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=40
LOCAL_C_INCLUDES += headers
include $(BUILD_EXECUTABLE)
