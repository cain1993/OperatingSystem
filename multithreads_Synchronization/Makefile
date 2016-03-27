obj-m += p4rjiang.o
all:
	gcc -o p4rjiang -lpthread p4rjiang.c -lm -I .
clean:
	make -C /usr/include M=$(PWD) clean
