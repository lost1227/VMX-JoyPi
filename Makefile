CC = g++
CFLAGS = -Wall -Werror -pedantic -g -Iinclude
INCLUDE = include
LINKERFLAGS = -L/usr/local/lib/vmxpi -lvmxpi_hal_cpp -lrt -lpthread -lm
VMXINCLUDE = -I/usr/local/include/vmxpi

BINS = main.o Xbox.o Joy.o SpeedController.o Utils.o DifferentialDrive.o TimedRobot.o Robot.o

all: joytest main

joytest: joytest.o Joy.o Xbox.o
	$(CC) $(CFLAGS) $(LINKERFLAGS) -o joytest joytest.o Joy.o Xbox.o

main: $(BINS)
	$(CC) $(CFLAGS) $(LINKERFLAGS) -o main $(BINS)

joytest.o: joytest.cpp $(INCLUDE)/Xbox.h
	$(CC) $(CFLAGS) -c joytest.cpp

main.o: main.cpp $(INCLUDE)/*.h
	$(CC) $(CFLAGS) $(VMXINCLUDE) -c main.cpp

Joy.o: Joy.cpp $(INCLUDE)/Joy.h $(INCLUDE)/Exceptions.h
	$(CC) $(CFLAGS) -c Joy.cpp

Xbox.o: Xbox.cpp $(INCLUDE)/Xbox.h $(INCLUDE)/Joy.h $(INCLUDE)/Exceptions.h
	$(CC) $(CFLAGS) -c Xbox.cpp

SpeedController.o: SpeedController.cpp $(INCLUDE)/SpeedController.h $(INCLUDE)/Exceptions.h
	$(CC) $(CFLAGS) $(VMXINCLUDE) -c SpeedController.cpp

Utils.o: Utils.cpp $(INCLUDE)/Utils.h
	$(CC) $(CFLAGS) -c Utils.cpp

DifferentialDrive.o: DifferentialDrive.cpp $(INCLUDE)/DifferentialDrive.h $(INCLUDE)/SpeedController.h $(INCLUDE)/Utils.h
	$(CC) $(CFLAGS) $(VMXINCLUDE) -c DifferentialDrive.cpp

TimedRobot.o: TimedRobot.cpp $(INCLUDE)/TimedRobot.h $(INCLUDE)/Exceptions.h
	$(CC) $(CFLAGS) $(VMXINCLUDE) -c TimedRobot.cpp

Robot.o: Robot.cpp $(INCLUDE)/*.h
	$(CC) $(CFLAGS) $(VMXINCLUDE) -c Robot.cpp

clean:
	rm -f *.o joytest main
