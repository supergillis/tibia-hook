CC=g++
SHAREDFLAGS=-fPIC -m32
CFLAGS=$(SHAREDFLAGS)
LDFLAGS=$(SHAREDFLAGS) -ldl -lpthread -L/usr/lib/i386-linux-gnu -lQtCore -lQtNetwork
INCLUDES= -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtGui -I/usr/include/qt4

all: clean main.o hook.o provider.o packet_handler.o thread.o message.o position.o
	$(CC) -shared $(LDFLAGS) main.o hook.o provider.o packet_handler.o thread.o message.o position.o -o hook.so
main.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) main.cpp
hook.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) hook.cpp
provider.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) provider.cpp
packet_handler.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) packet_handler.cpp
thread.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) thread.cpp
message.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) message.cpp
position.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) position.cpp
clean:
	-rm hook.so *.o
