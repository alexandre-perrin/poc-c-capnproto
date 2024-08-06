LIB_OBJS = \
	demo.capnp.o\

LIBS = \
	-lcapnp\

OBJS = \
	demo.o\
	$(LIB_OBJS)\

MY_CFLAGS = \
	-Wall\
	-Ic-capnproto/lib\
	-g -Og\

all: demo

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(MY_CFLAGS)

demo.capnp.c:
	capnp compile -o c demo.capnp

demo.o: demo.capnp.o

demo: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

run: demo
	./demo

clean:
	rm -f demo $(OBJS)
	rm -f demo.capnp.[ch]
