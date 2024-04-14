# -------------------------------------------------------------------------- #
#  libtsprites - terminal sprites library (for catimg images)      2023, M64 # 
#                                                  <m64.overdrive@gmail.com> #
# -------------------------------------------------------------------------- #

BINARY=test
# CC=zig c++
# CXX=zig c++

CXX=g++

CFLAGS+=-O3 -Wall --debug -lm -lSDL2
CXXFLAGS+=$(CFLAGS)

$(BINARY): test.cpp \
	../../tsprites.o \
	../../tsanimations.o \
	../../tscolors.o \
	../../tscreen.o \
	../../tseffects.o \
	../../tseffects.o \
	../../tsrender.o \
	../../tsrendersurface.o \
	../../tsutils.o  \
	../../lodepng.o
	$(CXX) -o $@ $^ -lm
#	$(RM) *.o ../../*.o ../../lib/*.o 

clean:
	$(RM) *.o $(BINARY) ../../*.o ../../lib/*.o 

.phony: clean

