.SUFFIXES:

CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++11 -pedantic -march=native -pipe
CXXFLAGS += -Wno-unused-parameter
CXXFLAGS += -g -Og -g3
#CXXFLAGS += -O3

AR = ar
ARFLAGS = rvs

#INCLUDE =  -Iinclude -I/usr/local/include/vmxpi
INCLUDE = -Iinclude -I/home/jordan/include/vmxpi

SRCDIR := src
BUILDDIR := build

SRCS = $(shell find $(SRCDIR) -name "*.cpp")
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
MAIN = libvmx.a

COMPILE.cpp = $(CXX) $(CXXFLAGS) $(TARGET_ARCH) -c

all: $(MAIN)

$(BUILDDIR)/%.d: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -MM -MP -MF $@ $(INCLUDE) $<

$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp $(BUILDDIR)/%.d | $(BUILDDIR)
	$(COMPILE.cpp) $(INCLUDE) $(OUTPUT_OPTION) $<

$(BUILDDIR) : ; @mkdir -p $(BUILDDIR)

$(MAIN) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

DEPFILES := $(SRCS:%.cpp=$(BUILDDIR)/%.d)
$(DEPFILES):
include $(wildcard $(DEPFILES))

clean:
	rm -rf $(BUILDDIR) $(MAIN)
