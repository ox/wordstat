wordstat: util.o hash.o main.c
	gcc -o wordstat util.o hash.o main.c

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

