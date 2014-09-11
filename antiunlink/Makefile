obj-m := android_module.o

KERNEL_DIR := $(HOME)/repo/android-kern/goldfish
CCPATH := $(ANDROID_NDK_ROOT)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/darwin-x86/bin/
CCPATH_EXT := $(CCPATH)arm-linux-androideabi-
EXTRA_CFLAGS=-fno-pic
ARCH=arm
SUBARCH=arm

# Get sys_call_table since we have kernel source anyway...
# grep "sys_call_table" System.map | sed "s/^\(.*\) \(. sys_call_table\)/0x\1/"
REPLACE_SYS_CALL_TABLE=$(shell ./get_sys_call_table.sh $(KERNEL_DIR) android_module.h)
LDFLAGS=-L$(KERNEL_DIR)/include/linux/

all:
	$(REPLACE_SYS_CALL_TABLE)
	make ARCH=arm CROSS_COMPILE=$(CCPATH_EXT) -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm -rf *.c~
	rm -rf *.h~
	rm -rf *.o
	rm -f modules.order