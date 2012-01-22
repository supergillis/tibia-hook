CC=g++
SHAREDFLAGS=-fPIC -m32
CFLAGS=$(SHAREDFLAGS) -c
LDFLAGS=$(SHAREDFLAGS) -shared -ldl
INCLUDES=

all: clean main.o hook.o provider.o packet_handler.o thread.o message.o position.o
	$(CC) $(LDFLAGS) $(INCLUDES) main.o hook.o provider.o packet_handler.o thread.o message.o position.o -o hook.so
main.o:
	$(CC) $(CFLAGS) $(INCLUDES) main.cpp
hook.o:
	$(CC) $(CFLAGS) $(INCLUDES) hook.cpp
provider.o:
	$(CC) $(CFLAGS) $(INCLUDES) provider.cpp
packet_handler.o:
	$(CC) $(CFLAGS) $(INCLUDES) packet_handler.cpp
thread.o:
	$(CC) $(CFLAGS) $(INCLUDES) thread.cpp
message.o:
	$(CC) $(CFLAGS) $(INCLUDES) message.cpp
position.o:
	$(CC) $(CFLAGS) $(INCLUDES) position.cpp
clean:
	-rm hook.so *.o
