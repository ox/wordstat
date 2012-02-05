wordstat: util.o hash.o wordstat.c
	gcc -o wordstat util.o hash.o wordstat.c

wordstat_debug: util.o hash.o main.c
	gcc -g -o wordstat util.c hash.c wordstat.c

util.o: util.c
	gcc -c util.c

hash.o: hash.c
	gcc -c hash.c

clean:
	rm wordstat
	rm *.o
