LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := runner
LOCAL_SRC_FILES := local2root/runner.c
LOCAL_LDLIBS    := -llog
LOCAL_C_INCLUDES := headers

include $(BUILD_SHARED_LIBRARY)

#include $(CLEAR_VARS)

#LOCAL_MODULE    := exploit
#LOCAL_SRC_FILES := exploit.c
#include $(BUILD_EXECUTABLE) 

include $(CLEAR_VARS)

LOCAL_MODULE    := suidext
LOCAL_SRC_FILES := suidext/suidext.c
LOCAL_C_INCLUDES := headers

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_MODULE    := expl_check
LOCAL_SRC_FILES := local2root/expl_check.c
LOCAL_C_INCLUDES := headers

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_MODULE    := local_exploit
LOCAL_SRC_FILES := local2root/local_exploit.c local2root/exploit_list.c local2root/kallsyms_in_memory.c local2root/kallsyms.c
LOCAL_C_INCLUDES := headers

include $(BUILD_EXECUTABLE)


