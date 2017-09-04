MODULE_NAME := netlink_test_kernel
obj-m :=$(MODULE_NAME).o

KERNEL_VER := $(shell uname -r)
KERNEL_DIR := /lib/modules/$(KERNEL_VER)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD)
	gcc -o netlink_test_user netlink_test_user.c
clean:
	rm -fr *.ko *.o *.cmd netlink_test_user $(MODULE_NAME).mod.c
