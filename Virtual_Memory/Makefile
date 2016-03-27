obj-m += p5rjiang.o
all:
	gcc -o p5rjiang -lpthread p5rjiang.c -lm -I .
clean:
	make -C /usr/include M=$(PWD) clean
