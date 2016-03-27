obj-m += p6rjiang.o
all:
	gcc -o p6rjiang  p6rjiang.c myApp.c disk.c -lm -I .
clean:
	make -C /usr/include M=$(PWD) clean
