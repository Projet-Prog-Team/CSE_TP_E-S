%.o:	
	gcc -c test.c stdes.h -g
	gcc -c stdes.c stdes.h -g

test1: test.o stdes.o
	gcc -o test1 test.o stdes.o -g

clean:
	rm *.o
	rm ./test1
