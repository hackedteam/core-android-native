LOCAL_PATH := $(call my-dir)
CPP_CORE := $(abspath $(call my-dir)/../../CPPCore)

include $(CLEAR_VARS)
LOCAL_MODULE    := selinux_exploit
LOCAL_SRC_FILES := remote_main/remote_main.c put_user_exploit/put_user_exploit.c  put_user_exploit/lib_put_user.c utils/kallsyms_in_memory.c utils/kallsyms.c utils/device_database.c utils/knox_manager.c utils/xperia_ric_manager.c utils/deobfuscate.c utils/log.c utils/utils.c utils/shell_installer.c utils/old_shell.c utils/boot_manager.c utils/ps.c put_user_exploit/check_put_user_exploit.c kernel_waiter_exploit/selinux4_exploit.c kernel_waiter_exploit/check.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DREMOTE -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=40 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=40
LOCAL_C_INCLUDES += headers
include $(BUILD_EXECUTABLE)
