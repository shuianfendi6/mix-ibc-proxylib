LOCAL_PATH :=$(call my-dir)

subdirs :=$(LOCAL_PATH)/ibc-proxylib/ibc-proxylib.mk
#subdirs +=$(LOCAL_PATH)/Test/test.mk

include $(subdirs)

