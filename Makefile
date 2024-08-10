PROTO_LIBRARY_NAME=rpi-protogen-display

RGB_LIBDIR=./rpi-rgb-led-matrix
PROTO_DIR=./src

PROTO_BINARY=./build/$(PROTO_LIBRARY_NAME)

all : $(PROTO_BINARY)

$(PROTO_BINARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)
	$(MAKE) -C $(PROTO_DIR)

clean:
	$(MAKE) -C $(RGB_LIBDIR) clean
	$(MAKE) -C $(PROTO_DIR) clean

FORCE:
.PHONY: FORCE
