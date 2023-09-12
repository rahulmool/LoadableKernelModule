obj-m := hello.o # Module Name is hello.c

KDIR := /usr/src/linux-headers-$(shell uname -r)

all: 
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean: 
	rm -rf *.o *.ko *.mod.* *.symvers *.order *.cmd .hello* .Module* .modules* 