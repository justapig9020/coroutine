all: main

main: main.o coroutine.o
	gcc -o main main.o coroutine.o

%.o: %.c coroutine.h
	gcc -c $< -o $@

clean:
	rm -f main *.o