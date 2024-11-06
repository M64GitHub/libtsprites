# -------------------------------------------------------------------------- #
#  libtsprites - terminal sprites library                          2023, M64 # 
#                                                  <m64.overdrive@gmail.com> #
# -------------------------------------------------------------------------- #

LIBRARY=tsprites

SRCDIR=src
LIBDIR=lib
SONAME=$(LIBRARY).so
SOFILE=$(LIBDIR)/$(SONAME)
LIBOBJFILE=$(LIBRARY).o
LIBINSTALLDIR=/usr/lib

# CC=zig c++
# CXX=zig c++

CXX=g++

CFLAGS+=-O3 -Wall 
CXXFLAGS+=$(CFLAGS)

lib: $(SOFILE)

# $(BINARY): test.o $(SOFILE)
# 	$(CXX) -o $@ $^ -Llib -l$(LIBRARY) --debug -lm

$(SOFILE): $(SRCDIR)/$(LIBRARY).cpp
	$(CXX) -fPIC -c $(SRCDIR)/$(LIBRARY).cpp -o $(SRCDIR)/$(LIBOBJFILE)
	$(CXX) -fPIC -c $(SRCDIR)/tscolors.cpp -o $(SRCDIR)/tscolors.o 
	$(CXX) -fPIC -c $(SRCDIR)/tscreen.cpp -o $(SRCDIR)/tscreen.o 
	$(CXX) -fPIC -c $(SRCDIR)/tsscener.cpp -o $(SRCDIR)/tsscener.o 
	$(CXX) -fPIC -c $(SRCDIR)/tsutils.cpp -o $(SRCDIR)/tsutils.o 
	$(CXX) -fPIC -c $(SRCDIR)/tsrender.cpp -o $(SRCDIR)/tsrender.o 
	$(CXX) -fPIC -c $(SRCDIR)/tseffects.cpp -o $(SRCDIR)/tseffects.o 
	$(CXX) -fPIC -c $(SRCDIR)/tscroller.cpp -o $(SRCDIR)/tscroller.o 
	$(CXX) -fPIC -c $(SRCDIR)/tsanimations.cpp -o $(SRCDIR)/tsanimations.o 
	$(CXX) -fPIC -c $(SRCDIR)/lodepng.cpp -o $(SRCDIR)/lodepng.o 
	$(CXX) -fPIC -c $(SRCDIR)/tsrendersurface.cpp \
		-o $(SRCDIR)/tsrendersurface.o 
	$(CXX) -shared -Wl,-soname,$(SONAME) -o $(SOFILE) \
		$(SRCDIR)/$(LIBOBJFILE) \
		$(SRCDIR)/tsanimations.o \
		$(SRCDIR)/tscolors.o \
		$(SRCDIR)/tscreen.o \
		$(SRCDIR)/tscroller.o \
		$(SRCDIR)/tseffects.o \
		$(SRCDIR)/tsrender.o \
		$(SRCDIR)/tsrendersurface.o \
		$(SRCDIR)/tsscener.o \
		$(SRCDIR)/tsutils.o

clean:
	$(RM) $(LIBDIR)/*.o $(LIBDIR)/*.so $(SRCDIR)/*.o

install:
	cp $(SOFILE) $(LIBINSTALLDIR)/ 

uninstall:
	$(RM) $(LIBINSTALLDIR)/$(SONAME)

.phony: all clean lib install uninstall

