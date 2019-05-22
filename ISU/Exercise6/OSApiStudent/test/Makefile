SRCS=main.cpp
BASEPATH=/home/stud/apps/OSApi/lib/host
# Determine whether this is a debug build or not
ifdef DEBUG
CFLAGS=-ggdb -O0
LIBPATH=$(BASEPATH)/debug
else
CFLAGS=-O2
LIBPATH=$(BASEPATH)/release
endif
# Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
CFLAGS+=-Wall -D_REENTRANT -DOS_LINUX -I$(BASEPATH)/inc

# Note how the flags are part of the compilation of .cpp file(s).
main.o: $(SRCS)
    g++ $(CFLAGS) -c main.cpp
# Then again, note how the flags are NOT part of the linking process
main: main.o
    g++ -o main main.o -L$(LIBPATH) -lOSApi -lrt -lpthread

all: main

clean:
    rm -f *.o main
