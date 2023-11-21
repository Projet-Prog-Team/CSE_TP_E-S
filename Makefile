EXEC=test

$(EXEC): 
	gcc -c stdes.c stdes.h -g
	gcc -c $@.c stdes.h -g
	gcc -o $@ $@.o stdes.o -g

clean:
	rm *.o
	rm ./$(EXEC)
