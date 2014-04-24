CC=clang
CFLAGS= -fPIC -I.
LDFLAGS=-shared
DEPS = libovc.h
OBJ = autocharge.o credit.o index.o info.o ovdata.o preamble.o slots.o subscription.o transaction.o utils.o
TARGET = libovc.so

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm *.o *.so
