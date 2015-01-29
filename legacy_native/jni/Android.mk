LOCAL_PATH := $(call my-dir)
SHARED := ../../shared

include $(CLEAR_VARS)

LOCAL_MODULE    := runner
LOCAL_SRC_FILES := local2root/runner.c
LOCAL_LDLIBS    := -llog
LOCAL_C_INCLUDES := headers $(SHARED)/headers

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := suidext
LOCAL_SRC_FILES := suidext/suidext.c $(SHARED)/sqlite_stuff/sqlite3_manager.c $(SHARED)/utils/utils.c $(SHARED)/utils/log.c $(SHARED)/utils/deobfuscate.c
LOCAL_C_INCLUDES := headers $(SHARED)/headers
LOCAL_CFLAGS := -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=40 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=40

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_MODULE    := expl_check
LOCAL_SRC_FILES := local2root/expl_check.c
LOCAL_C_INCLUDES := headers $(SHARED)/headers
LOCAL_CFLAGS := -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=40 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=40

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_MODULE    := local_exploit
LOCAL_SRC_FILES := local2root/local_exploit.c local2root/exploit_list.c local2root/kallsyms_in_memory.c local2root/kallsyms.c
LOCAL_C_INCLUDES := headers $(SHARED)/headers
LOCAL_CFLAGS := -w -mllvm -sub -mllvm -perSUB=100 -mllvm -fla -mllvm -perFLA=40 -mllvm -bcf -mllvm -perBCF=100 -mllvm -boguscf-prob=40

include $(BUILD_EXECUTABLE)


