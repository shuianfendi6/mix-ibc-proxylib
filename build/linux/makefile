######################################
# Copyright (c) 1997 George Foot (george.foot@merton.ox.ac.uk)
# All rights reserved.
######################################
#目标（可执行文档）名称，库（譬如stdcx,iostr,mysql等），头文件路径



DESTINATION := main
DESTINATION_SO := libibc-proxylib.so
DESTINATION_A := libibc-proxylib.a
LIBS := 
LOCAL_PATH := ../../src/ibc-proxylib
INCLUDES := .

LOCAL_C_INCLUDES := 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/miracl-lib
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/ibc-proxylib

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

###ibc-proxylib
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/sm3.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/sm4.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/ibc-proxylib/sm9_proxylib_api.c
LOCAL_SRC_FILES_CPP := $(LOCAL_PATH)/ibc-proxylib/big.cpp
LOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/ecn.cpp
LOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/ecn2.cpp
lOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/flash.cpp
LOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/zzn.cpp
LOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/zzn12a.cpp
LOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/zzn2.cpp
LOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/zzn4.cpp
LOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/sm9_proxylib_hw.cpp
LOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/sm9_proxylib_sw.cpp
LOCAL_SRC_FILES_CPP += $(LOCAL_PATH)/ibc-proxylib/sm9_proxylib_utilities.cpp

ifeq ($(BUILD32),1)
#LOCAL_SRC_FILES_AS := $(LOCAL_PATH)/nspr/pr/src/md/unix/os_Linux_x86.s
LOCAL_CFLAGS := -m32 
else
#LOCAL_SRC_FILES_AS := $(LOCAL_PATH)/nspr/pr/src/md/unix/os_Linux_x86_64.s
LOCAL_CFLAGS := -m64
LOCAL_CFLAGS += -DMIX_BUILD_FOR_64
endif

LOCAL_CFLAGS += $(LOCAL_C_INCLUDES) -fPIC -DMR_PAIRING_BN -DMR_NOASM -D__UNIX__
LOCAL_CFLAGS += -D_GNU_SOURCE=1 
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl -lpthread -lm

RM := rm -rf
#C,CC或CPP文件的后缀
PS=c
# GNU Make的隐含变量定义
CC=gcc
CXX = g++
CPPFLAGS = -Wall -O3
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += $(LOCAL_CFLAGS)
CPPFLAGS += -MMD

#以下部分无需修改
#SOURCE := $(wildcard *.$(PS)) @(wildcard $(LOCAL_SRC_FILES))
SOURCE=$(LOCAL_SRC_FILES)
OBJS := $(patsubst %.$(PS),%.o,$(SOURCE))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.$(PS),$(MISSING_DEPS)))

LOCAL_OBJ_FILES_AS := $(patsubst %.s,%.o,$(LOCAL_SRC_FILES_AS))
LOCAL_OBJ_FILES_CPP := $(patsubst %.cpp,%.o,$(LOCAL_SRC_FILES_CPP))

.PHONY : all deps objs clean rebuild

all : $(DESTINATION_A)  $(DESTINATION_SO) $(DESTINATION)

deps : $(DEPS)
	$(CC) -MM -MMD $(SOURCE)
 
show:
	echo $(SOURCE)
	echo $(LOCAL_CFLAGS)
	echo $(CPPFLAGS)

$(OBJS) :

$(LOCAL_OBJ_FILES_AS) : $(LOCAL_SRC_FILES_AS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@
	

$(LOCAL_OBJ_FILES_CPP) : $(LOCAL_SRC_FILES_CPP)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@

clean :
	@$(RM) $(LOCAL_OBJ_FILES_AS)
	@$(RM) $(LOCAL_OBJ_FILES_CPP)
	@$(RM) $(OBJS)
	@$(RM) $(DEPS)
	@$(RM) $(DESTINATION)

rebuild: clean all 
	ifneq ($(MISSING_DEPS),)
		$(MISSING_DEPS) :
			@$(RM) $(patsubst %.d,%.o,$@)
	endif

-include $(DEPS)

$(DESTINATION_SO) : $(OBJS) $(LOCAL_OBJ_FILES_AS) $(LOCAL_OBJ_FILES_CPP)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -fPIC -shared -o $(DESTINATION_SO) $(LOCAL_OBJ_FILES_AS) $(LOCAL_SRC_FILES_CPP) $(OBJS) $(addprefix -l,$(LIBS)) $(LOCAL_LDLIBS)
$(DESTINATION_A) : $(OBJS) $(LOCAL_OBJ_FILES_AS) $(LOCAL_OBJ_FILES_CPP)
	ar rcs $@ $^
$(DESTINATION) : $(DESTINATION_SO) $(DESTINATION_A)  
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o $(DESTINATION) $(LOCAL_PATH)/ibc-proxylib-test/sm9main.c -L. -libc-proxylib  $(addprefix -l,$(LIBS)) $(LOCAL_LDLIBS)
	#$(CXX) $(CFLAGS) $(CPPFLAGS) -o $(DESTINATION) $(LOCAL_PATH)/ibc-proxylib-test/sm9main.c $(DESTINATION_A)  $(addprefix -l,$(LIBS)) $(LOCAL_LDLIBS)





#结束

######################################
#    原作者是Gorge Foot，写这个Makefile的时候还是一个学生 
#    ":="赋值，和"="不同的是，":="在赋值的同时，会将赋值语句中所有的变量就地展开，也就是说，A:=$(B)后，B的值的改变不再影响A 
#    隐含规则。GUN Make在不特别指定的情况下会使用诸如以下编译命令：$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@，这也是为什么这个Makefile最后一个命令没有添加$(CPPFLAGS)的原因，因为缺省是包含这个变量的
#    函数和变量很相似："$ (函数名，空格，一列由逗号分隔的参数)" 
#    SOURCES = $(wildcard *.cpp) 列出工作目录下文件名满足"*.cpp"条件的文件，以空格分隔，并将列表赋给SOURCE变量 
#    patsubst函数：3个参数。功能是将第三个参数中的每一项（由空格分隔）符合第一个参数描述的部分替换成第二个参数制定的值 
#    addprefix函数：2个参数。将源串（第2个参数，由空格分隔）中的每一项添加前缀（第1个参数） 
#    filter-out函数：2个参数。从第二串中过滤掉包含在第一个串中的项 
#    $(CC) -MM -MMD $(SOURCE) : 对每个源文件生成依赖(dependence，Make通过依赖规则来判断是否需要重新编译某个文件)，"D"生成".d"文件，-MM表示去掉 depends里面的系统的头文件(使用<>包含的头文件)（若使用-M则全部包含，事实上，系统头文件被修改的可能性极小，不需要执行依赖检查）
#    .PHONY，不检查后面制定各项是否存在同名文件 
#    ifneg...else...endif，Makefile中的条件语句 
#    -include $(DEPS) : 将DEPS中的文件包含进来，"-"表示忽略文件不存在的错误 
#    @$(RM) *.o : 开头的"@"表示在Make的时候，不显示这条命令（GNU Make缺省是显示的) 
#    all : 作为第一个出现的目标项目，Make会将它作为主要和缺省项目("make"就表示"make all") 
#    deps : 只生成依赖文件(.d文件) 
#    objs : 为每一个源码程序生成或更新 '.d' 文件和'.o'文件 
#    clean : 删除所有'.d','.o'和可执行文件 
#    rebuild : clean然后重建 
#    内部变量$@, $< $^ : 分别表示目标名(:前面的部分，比如all)，依靠列表（:后面的部分）中的第一个依靠文件，所有依靠文件 
######################################
