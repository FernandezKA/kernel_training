obj-m += hello.o

# Get current directory path
SRC := $(shell pwd)

# Check if KERNELDIR is defined
ifndef KERNELDIR
	KERNELDIR := /lib/modules/$(shell uname -r)/build
endif

.PHONY: all clean

all:
	$(MAKE) -C $(KERNELDIR) M=$(SRC) modules

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c *.mod
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
