# -------------------------------------------------------------------------- #
#  libtsprites - terminal sprites library (for catimg images)      2023, M64 # 
#                                                  <m64.overdrive@gmail.com> #
# -------------------------------------------------------------------------- #

BINARY=test
LIBRARY=tsprites

SONAME=lib$(LIBRARY).so
SOFILE=lib/$(SONAME)
LIBOBJFILE=lib/$(LIBRARY).o
LIBINSTALLDIR=/usr/lib

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
	$(CXX) -fPIC -c tsrender.cpp -o lib/tsrender.o 
	$(CXX) -fPIC -c tseffects.cpp -o lib/tseffects.o 
	$(CXX) -fPIC -c tsanimations.cpp -o lib/tsanimations.o 
	$(CXX) -fPIC -c tsrendersurface.cpp -o lib/tsrendersurface.o 
	$(CXX) -shared -Wl,-soname,$(SONAME) -o $(SOFILE) $(LIBOBJFILE) \
		lib/tscolors.o lib/tscreen.o lib/tsutils.o lib/tsrender.o \
		lib/tseffects.o lib/tsanimations.o lib/tsrendersurface.o

clean:
	$(RM) *.o lib/*.o lib/*.so $(BINARY)

install:
	cp $(SOFILE) $(LIBINSTALLDIR)/ 

uninstall:
	$(RM) $(LIBINSTALLDIR)/$(SONAME)

.phony: all clean lib install uninstall

