# Makefile to build Xenomai Kernel (Cobalt+Dovetail) PC SPeaker RDTM Driver

ifneq ($(KERNELRELEASE),)
# here are options when make called recursively from kernel targets
obj-m   :=      xeno_spkr.o
ifeq ($(debug),1)
# enable pr_debug() output - from: https://stackoverflow.com/a/28941476
CFLAGS_xeno_spkr.o := -DDEBUG
endif
else
# here are options when make invoked directly
KDIR := /lib/modules/$(shell uname -r)/build 

ifeq ($(debug),1)
# enable pr_debug() output - from: https://stackoverflow.com/a/28941476
CFLAGS_xeno_spkr.o := -DDEBUG
endif
endif

EXTRA_CFLAGS += -I/usr/include/xenomai

all:
	make -C $(KDIR) M=$(PWD) modules debug=$(debug)

clean:
	make -C $(KDIR) M=$(PWD) clean

.PHONY: all clean
