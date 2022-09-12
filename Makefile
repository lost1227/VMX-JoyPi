.SUFFIXES:

CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++11 -pedantic -march=native -pipe
CXXFLAGS += -g -Og -g3
#CXXFLAGS += -O3

INCLUDE =  -Ilibvmx/include -I/home/jordan/include/vmxpi
#INCLUDE =  -Ilibvmx/include -I/usr/local/include/vmxpi

LINKERFLAGS = -L/usr/local/lib/vmxpi -lvmxpi_hal_cpp -lrt -lpthread -lm -Wl,-O1

LIBVMX := libvmx/libvmx.a

COMPILE.cpp = $(CXX) $(CXXFLAGS) -c

all: joytest main

joytest: joytest.o $(LIBVMX)
	$(CXX) $(CXXFLAGS) $(LINKERFLAGS) joytest.o $(LIBVMX) -o joytest

main: main.o $(LIBVMX)
	$(CXX) $(CXXFLAGS) $(LINKERFLAGS) main.o $(LIBVMX) -o main
	sudo chown root:root main
	sudo chmod 4755 main

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MP -MF $@ $(INCLUDE) $<

%.o : %.cpp %.d
	$(COMPILE.cpp) $(INCLUDE) $(OUTPUT_OPTION) $<

DEPFILES := joytest.d main.d
$(DEPFILES):
include $(wildcard $(DEPFILES))

clean:
	rm -f *.o *.d joytest main
