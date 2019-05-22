# This makefile is shamelessly stolen from "Brodersalsa"
ifeq ($(ARCH),THREAD)
TEST=TestThread
endif
ifeq ($(ARCH),LOG)
TEST=TestLog
endif
ifeq ($(ARCH),TIME)
TEST=TestTime
endif
ifeq ($(ARCH),TIMER)
TEST=TestTimer
endif
SRCS=@(TEST:=.cpp)
OBJS=$(TEST:=.o)
BASEPATH=..
ifeq ($(TARGET),host)
CXX = g++
EXEC_DIR = bin/host
MKDIR = mkdir -p
# Determine whether this is a debug build or not
ifdef DEBUG
CXXFLAGS=-ggdb -O0
LIBPATH=$(BASEPATH)/lib/host/debug
else
CXXFLAGS=-O2
LIBPATH=$(BASEPATH)/lib/host/release
endif
endif
ifeq ($(TARGET),target)
CXX = arm-rpizw-g++
EXEC_DIR =bin/target
MKDIR =mkdir -p
# Determine whether this is a debug build or not
ifdef DEBUG
CXXFLAGS=-ggdb -O0
LIBPATH=$(BASEPATH)/lib/target/debug
else
CXXFLAGS=-O2
LIBPATH=$(BASEPATH)/lib/target/release
endif
endif
## Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
CXXFLAGS+=-Wall -D_REENTRANT -DOS_LINUX -I$(BASEPATH)/inc
#${BUILD_DIR}/%.o : %.cpp
#	$(MKDIR) $(BUILD_DIR)
#	$(CXX) $(CXXFLAGS) -c -o $@ $^
# Then again, note how the flags are NOT part of the linking process
${TEST}: $(OBJS)
	@$(MKDIR) $(EXEC_DIR)
	$(CXX) -o $(EXEC_DIR)/$@ $(OBJS) -L$(LIBPATH) -lOSApi -lrt -lpthread
all: $(TEST)
ifeq ($(CLEAN),bin)
RMV=bin
endif
clean:$(OBJS)
	rm -fr *.o $(RMV)
