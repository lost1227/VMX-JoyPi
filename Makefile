CC = g++
CFLAGS = -Wall -Werror -pedantic -g -Iinclude
INCLUDE = include
LINKERFLAGS = -L/usr/local/lib/vmxpi -lvmxpi_hal_cpp -lrt -lpthread -lm
VMXINCLUDE = -I/usr/local/include/vmxpi

all: joytest main

joytest: joytest.o Joy.o Xbox.o
	$(CC) $(CFLAGS) $(LINKERFLAGS) -o joytest joytest.o Joy.o Xbox.o

main: main.o Xbox.o Joy.o SpeedController.o Utils.o
	$(CC) $(CFLAGS) $(LINKERFLAGS) -o main main.o Xbox.o Joy.o SpeedController.o Utils.o

joytest.o: joytest.cpp $(INCLUDE)/Xbox.h
	$(CC) $(CFLAGS) -c joytest.cpp

main.o: main.cpp $(INCLUDE)/Xbox.h $(INCLUDE)/Joy.h $(INCLUDE)/SpeedController.h $(INCLUDE)/Exceptions.h $(INCLUDE)/Utils.h
	$(CC) $(CFLAGS) $(VMXINCLUDE) -c main.cpp

Joy.o: Joy.cpp $(INCLUDE)/Joy.h $(INCLUDE)/Exceptions.h
	$(CC) $(CFLAGS) -c Joy.cpp

Xbox.o: Xbox.cpp $(INCLUDE)/Xbox.h $(INCLUDE)/Joy.h $(INCLUDE)/Exceptions.h
	$(CC) $(CFLAGS) -c Xbox.cpp

SpeedController.o: SpeedController.cpp $(INCLUDE)/SpeedController.h $(INCLUDE)/Exceptions.h
	$(CC) $(CFLAGS) $(VMXINCLUDE) -c SpeedController.cpp

Utils.o: Utils.cpp $(INCLUDE)/Utils.h
	$(CC) $(CFLAGS) -c Utils.cpp

clean:
	rm -f *.o joytest main
