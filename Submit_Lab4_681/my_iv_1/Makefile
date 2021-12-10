# sample makefile using Coin3D library

COIN_DIRECTORY = /usr/local/Coin3d
COIN_INCLUDE = -I$(COIN_DIRECTORY)/include -I.
COIN_LIBRARIES = -L$(COIN_DIRECTORY)/lib

CPP = /usr/local/bin/g++  -R$(COIN_DIRECTORY)/lib
INCLUDES = $(COIN_INCLUDE) 

# Uncomment to turn on debugging:
CPPFLAGS = -g -DDEBUG

LIBRARIES =  $(COIN_LIBRARIES) -lCoin 

OBJECT_FILES = OSUInventor.o rt.o
EXEC = rt

.C.o:
	$(CPP) -c $(CPPFLAGS) $(INCLUDES) $<

lab1: $(OBJECT_FILES)
	$(CPP) -o $(EXEC) $(CPPFLAGS) $(OBJECT_FILES) \
        $(LDFLAGS) $(LIBRARIES)

clean:
	rm *.o $(EXEC)
