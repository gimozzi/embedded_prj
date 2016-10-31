obj-m	:= stopwatch.o

#KDIR	:=  /home/lee/Desktop/kernel
KDIR	:= /root/5week/kernel
#KDIR	:= /root/5week/hw2_temp
PWD		:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -rf *.o
	rm -rf stopwatch.ko
	rm -rf *.mod.c
	rm -rf *.order
	rm -rf *.symvers
