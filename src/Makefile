PROTO_LIBRARY_NAME=rpi-protogen-display

CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS)
OBJECTS=$(PROTO_LIBRARY_NAME)-main.o
BINARIES=$(PROTO_LIBRARY_NAME)

RGB_LIB_DISTRIBUTION=../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

REDIS_LIBRARY_NAME=hiredis
LDFLAGS+=-l$(REDIS_LIBRARY_NAME) -lrt -lm -lpthread

JSON_LIBRARY_NAME=jsoncpp
LDFLAGS+=-l$(JSON_LIBRARY_NAME) -lrt -lm -lpthread

LOTTIE_LIBRARY_NAME=rlottie
LDFLAGS+=-l$(LOTTIE_LIBRARY_NAME) -lrt -lm -lpthread

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

$(PROTO_LIBRARY_NAME) : $(PROTO_LIBRARY_NAME)-main.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)
	ln -s ../src/$(BINARIES) ../build/$(BINARIES)

% : %.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

%.o : %.c
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)
	rm -f ../build/$(BINARIES)

FORCE:
.PHONY: FORCE
