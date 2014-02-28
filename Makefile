CC := gcc
CFLAGS := -I./ -ansi -pedantic
LDFLAGS := -L./
LDLIBS := -lm -lisland

all: CFLAGS += -O3
all: lib island

debug: CFLAGS += -g
debug: lib island

lib: libisland.a

libisland.a: island.o
	ar rcs libisland.a island.o

island: main.o

main.o: main.c island.h

island.o: island.c island.h

clean:
	$(RM) *.o *.a island
