#define exec name
EXEC=prog
#define compiler flags
CXXFLAGS =-I. -lpthread
#define linker flags 
LXXFLAGS =-lpthread
#define default compiler HOST
CXX=g++
#define default build dir
BUILD_DIR=build/host
#define default path for the executeable
BIN_DIR=bin/host
#check for target specific build option - should be rpizx istread of target. eg. make ARCH=rpizw
ifeq (${ARCH},target) 
CXX=arm-rpizw-g++
BUILD_DIR=build/target
BIN_DIR=bin/target
endif
#define project src NOTE the addprefix to define build path
SOURCES= $(addprefix ${BUILD_DIR}/,$(wildcard *.cpp))  
#convert %.cpp files to %.o files.
OBJECTS = ${SOURCES:.cpp=.o}
#add dependency files (*.h,*.hpp) 
DEPS = ${SOURCES:.cpp=.d}
#create desitnation folder, note:do nothing if exsists
$(shell mkdir -p -m a=rwx ${BUILD_DIR})
$(shell mkdir -p -m a=rwx ${BIN_DIR})
#define default, re-route
all:${EXEC}
#define EXEC specficic
${EXEC}: ${DEPS} ${OBJECTS}
	@echo [Linking] $@
	@${CXX} -o ${EXEC} ${OBJECTS} ${LXXFLAGS}
	@echo Done.
#create directe link for o=cpp
${BUILD_DIR}/%.o : %.cpp
	@echo [Compiling] $^
	@${CXX} -c $^ -o $@ ${CXXFLAGS}
${BUILD_DIR}/%.d : %.cpp
	@echo [listing dependencys] $@
	@${CXX} -MT${@:.d=.o} -MM ${CXXFLAGS} $^ > $@
#utils
clean:
	@rm -rf build
	@rm -rf bin
	@echo Done.
help:
	@echo Targets: all, clean, help
