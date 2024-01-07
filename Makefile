# -------------------------------------------------------------------------- #
#  libtsprites - terminal sprites library (for catimg images)      2023, M64 # 
#                                                  <m64.overdrive@gmail.com> #
# -------------------------------------------------------------------------- #

BINARY=test
LIBRARY=tsprites

SONAME=lib$(LIBRARY).so
SOFILE=lib/$(SONAME)
LIBOBJFILE=lib/$(LIBRARY).o

# CC=zig c++
# CXX=zig c++

CXX=g++

CFLAGS+=-O3 -Wall 
CXXFLAGS+=$(CFLAGS)

all: $(BINARY)

lib: $(SOFILE)

$(BINARY): test.o $(SOFILE)
	$(CXX) -o $@ $^ -Llib -l$(LIBRARY) --debug -lm

$(SOFILE): $(LIBRARY).cpp
	$(CXX) -fPIC -c $(LIBRARY).cpp -o $(LIBOBJFILE)
	$(CXX) -fPIC -c tscolors.cpp -o lib/tscolors.o 
	$(CXX) -fPIC -c tscreen.cpp -o lib/tscreen.o 
	$(CXX) -fPIC -c tsutils.cpp -o lib/tsutils.o 
	$(CXX) -shared -Wl,-soname,$(SONAME) -o $(SOFILE) $(LIBOBJFILE) \
		lib/tscolors.o lib/tscreen.o lib/tsutils.o

clean:
	$(RM) *.o lib/*.o lib/*.so $(BINARY)

.phony: clean

