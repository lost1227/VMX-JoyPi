CC = g++
CFLAGS = -Wall -Werror -pedantic -g

all: joytest

joytest: main.o Joy.o
	$(CC) $(CFLAGS) -o joytest main.o Joy.o

main.o: main.cpp Joy.h
	$(CC) $(CFLAGS) -c main.cpp

Joy.o: Joy.cpp Joy.h
	$(CC) $(CFLAGS) -c Joy.cpp

clean:
	rm -f *.o joytest
