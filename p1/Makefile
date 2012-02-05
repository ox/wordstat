wordstat: util.o hash.o main.c
	gcc -o wordstat util.o hash.o main.c

wordstat_debug: util.o hash.o main.c
	gcc -g -o wordstat util.c hash.c main.c

util.o: util.c
	gcc -c util.c

hash.o: hash.c
	gcc -c hash.c

install: wordstat
	cp wordstat /usr/local/bin/

clean:
	rm wordstat
	rm /usr/local/bin/wordstat
	rm *.o

