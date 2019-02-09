all: process

process: process.o
	gcc process.o -o process

process.o:
	gcc -c process.c

clean:
	rm -f *o process *.gt
