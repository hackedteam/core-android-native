LOCAL_PATH := $(call my-dir)
CPP_CORE := $(abspath $(call my-dir)/../../CPPCore)
SHARED := ../../shared

include $(CLEAR_VARS)
LOCAL_MODULE    := selinux_exploit
LOCAL_SRC_FILES := put_user_exploit/put_user_exploit.c  put_user_exploit/lib_put_user.c utils/kallsyms_in_memory.c utils/kallsyms.c utils/device_database.c utils/knox_manager.c utils/xperia_ric_manager.c $(SHARED)/utils/deobfuscate.c $(SHARED)/utils/log.c $(SHARED)/utils/utils.c utils/shell_installer.c utils/old_shell.c utils/boot_manager.c utils/ps.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DLOCAL -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=80 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=80
LOCAL_C_INCLUDES += headers $(SHARED)/headers
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := selinux_check
LOCAL_SRC_FILES := put_user_exploit/check_put_user_exploit.c put_user_exploit/lib_put_user.c $(SHARED)/utils/log.c $(SHARED)/utils/deobfuscate.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DLOCAL -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=80 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=80
LOCAL_C_INCLUDES += headers $(SHARED)/headers
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := selinux4_exploit
LOCAL_SRC_FILES := kernel_waiter_exploit/selinux4_exploit.c kernel_waiter_exploit/check.c utils/knox_manager.c utils/xperia_ric_manager.c $(SHARED)/utils/deobfuscate.c $(SHARED)/utils/log.c $(SHARED)/utils/utils.c utils/boot_manager.c utils/ps.c utils/shell_installer.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DLOCAL -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=80 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=80
LOCAL_C_INCLUDES += headers $(SHARED)/headers
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := selinux4_check
LOCAL_SRC_FILES := kernel_waiter_exploit/selinux4_check.c kernel_waiter_exploit/check.c $(SHARED)/utils/log.c $(SHARED)/utils/deobfuscate.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DLOCAL -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=80 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=80
LOCAL_C_INCLUDES += headers $(SHARED)/headers
include $(BUILD_EXECUTABLE)
