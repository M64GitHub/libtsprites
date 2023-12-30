# -------------------------------------------------------------------------- #
#  libtsprites - terminal sprites library (for catimg images)      2023, M64 # 
#                                                  <m64.overdrive@gmail.com> #
# -------------------------------------------------------------------------- #

BINARY=test
LIBRARY=tsprite
# CC=zig c++
# CXX=zig c++

CXX=g++

CFLAGS+=-O3 -Wall --debug  -lm
CXXFLAGS+=$(CFLAGS)

$(BINARY): test.o tsprites.o
	$(CXX) -o $@ $^ 

clean:
	$(RM) *.o $(BINARY)

.phony: clean

