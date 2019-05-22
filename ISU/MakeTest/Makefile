# MakeFile for generel purpose compilation 
#
# Copyright (C) 2018 Mikkel Steen Sørensen - $ mikkel-steen@hotmail.com
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without #restriction.
# Including without limitation the rights to use, copy, modify, merge, publish, distribute, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, #subject to the following conditions:
#
# THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE.
#
# the software is provided "as is", without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose and #noninfringement.
# in no event shall the authors or copyright holders be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in #connection with the software or the use or other dealings in the software.
 

#name executeable same as folder.
BIN_NAME := $(notdir $(shell pwd))
#define output path (suffixed for collision control)
OUTPUT   := build_
#query source files
SOURCES  := $(subst ./,,$(shell find -name '*.c' -o -name '*.cpp'))
#query object files
OBJECTS  := $(patsubst %.cpp, ${OUTPUT}/%.o, ${SOURCES})
#Clang dependencies setup
DEP 	 := $(OBJECTS:%.o=%.d)
#include dependencies
-include $(DEP)
#define compiler
CXX	 := g++
#define common CFLAGS
CFLAGS 	 := -c
#define common LFLAGS
LDFLAGS  := -lm
#create folder for current build.
$(shell mkdir -p -m a=rwx ${OUTPUT})
#define tgr:rule?action
all: $(BIN_NAME)
${BIN_NAME}:${OBJECTS}
	@echo "[Linking] "$@
	@${CXX} ${LDFLAGS} ${OBJECTS} -o ${OUTPUT}/$@
	@echo "done, executeable is located @ "${OUTPUT}/$@
.PHONY : clean	
clean:
	@rm -rf $(OUTPUT)
	@echo "Done."
${OUTPUT}/%.o : %.cpp
	@echo "[Compiling] "$@
	@mkdir -p -m a=rwx ${dir $@}
	@${CXX} ${CFLAGS} $^ -o $@ 

	
	
	
	