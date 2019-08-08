CC = g++
CFLAGS = -Wall -Werror -pedantic -g

all: joytest

joytest: main.o Joy.o Xbox.o
	$(CC) $(CFLAGS) -o joytest main.o Joy.o Xbox.o

main.o: main.cpp Joy.h
	$(CC) $(CFLAGS) -c main.cpp

Joy.o: Joy.cpp Joy.h
	$(CC) $(CFLAGS) -c Joy.cpp

Xbox.o: Xbox.cpp Xbox.h Joy.h
	$(CC) $(CFLAGS) -c Xbox.cpp

clean:
	rm -f *.o joytest
