# Names: Stanley Vossler, Carlos Pantoja-Malaga, Matthew Echenique
# Professor: Andy Wang, PhD
# Class: COP 4610
# Project: 2
# Description: This is the makefile for 2nd part of Project 2: "Kernel Module Programming"

obj-m += my_timer.o

PWD := `pwd`
KERNELDIR := /lib/modules/`uname -r`/build

default:
        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
        rm -f *.ko *.o Module* *mod*
