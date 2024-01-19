# -------------------------------------------------------------------------- #
#  libtsprites - terminal sprites library (for catimg images)      2023, M64 # 
#                                                  <m64.overdrive@gmail.com> #
# -------------------------------------------------------------------------- #

BINARY=rendertest
# CC=zig c++
# CXX=zig c++

CXX=g++

CFLAGS+=-O3 -Wall --debug  -lm
CXXFLAGS+=$(CFLAGS)

$(BINARY): rendertest.o ../../tsprites.o \
../../tsanimations.o \
../../tscolors.o \
../../tscreen.o \
../../tseffects.o \
../../tseffects.o \
../../tsrender.o \
../../tsrendersurface.o \
../../tsutils.o

	$(CXX) -o $@ $^ 

clean:
	$(RM) *.o $(BINARY) ../../*.o ../../lib/*.o ../../lib/*.so

.phony: clean

