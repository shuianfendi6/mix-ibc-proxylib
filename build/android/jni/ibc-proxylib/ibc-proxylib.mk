LOCAL_PATH :=$(call my-dir)/../../../../src/ibc-proxylib

include $(CLEAR_VARS)

LOCAL_MODULE := libibc-proxy

LOCAL_CPP_EXTENSION := .cxx .cpp .cc  

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/miracl-lib
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/ibc-proxylib
LOCAL_C_INCLUDES += -I/usr/include

###miracl
LOCAL_SRC_FILES := $(LOCAL_PATH)/miracl-lib/mralloc.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrarth0.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrarth1.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrarth2.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrarth3.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrbits.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrbuild.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrcore.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrcurve.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrdouble.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrebrick.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrec2m.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrecn2.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrfast.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrflash.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrflsh1.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrflsh2.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrflsh3.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrflsh4.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrgcd.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrgf2m.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrio1.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrio2.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrjack.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrlucas.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrmonty.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrpi.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrpower.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrprime.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrrand.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrround.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrscrt.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrsmall.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrsroot.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrstrong.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrxgcd.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrzzn2.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrzzn4.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrzzn2b.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrsha3.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrshs.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrshs256.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/miracl-lib/mrshs512.c

ifeq ($(TARGET_ARCH), arm)

endif

###ibc-proxylib
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/big.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/ecn.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/ecn2.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/flash.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/sm3.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/sm4.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/zzn.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/zzn12a.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/zzn2.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/zzn4.cpp

LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/sm9_proxylib_hw.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/sm9_proxylib_sw.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/sm9_proxylib_utilities.cpp
ifeq ($(TARGET_ARCH), arm)

else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)

endif

ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
#LOCAL_CFLAGS += -DMIX_BUILD_FOR_64
endif

LOCAL_CFLAGS += $(LOCAL_C_INCLUDES) -DMR_PAIRING_BN -DMR_NOASM -D__UNIX__
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)

include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
#-------------------------lib-ibc-------------------------#


#-------------------------lib-ibc-------------------------#
include $(CLEAR_VARS) 

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/miracl-lib
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/ibc-proxylib
LOCAL_C_INCLUDES += -I/usr/include

LOCAL_CPP_EXTENSION := .cxx .cpp .cc  

LOCAL_MODULE    := lib-sm9-$(TARGET_ARCH_ABI)
#LOCAL_MODULE    := lib-sm9

LOCAL_SRC_FILES := $(LOCAL_PATH)/ibc-proxylib/sm9_proxylib_api.cpp

LOCAL_STATIC_LIBRARIES := libibc-proxy

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

LOCAL_CFLAGS += $(LOCAL_C_INCLUDES) -DMR_PAIRING_BN -DMR_NOASM -D__UNIX__
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)

include $(BUILD_SHARED_LIBRARY)






