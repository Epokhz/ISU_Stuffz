SOURCES=main.cpp part1.cpp part2.cpp

OBJECTS=${SOURCES:-cpp=.o}

EXECUTABLE=RUN

CXX=g++

all:${EXECUTABLE}

${EXECUTABLE}:${OBJECTS}
	${CXX} -o ${EXECUTABLE} ${OBJECTS}

%.o:%.hpp

clean:
	rm -rf *.o && rm ${EXECUTABLE}
	@echo Done

help :
	@echo You can make:
	@echo all
	@echo clean
	@echo help
