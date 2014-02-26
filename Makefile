LDLIBS = -lm

all: CFLAGS = -O3 -ansi -pedantic
all: ds

debug: CFLAGS = -g -ansi -pedantic
debug: ds

ds: main.o ds.o

main.o: main.c ds.h

ds.o: ds.c ds.h

clean:
	$(RM) *.o ds
