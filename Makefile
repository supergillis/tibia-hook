CC=g++
MOC=moc

CFLAGS=-fPIC -m32
LDFLAGS=-m32 -ldl -lpthread -L/usr/lib/i386-linux-gnu -lQtCore -lQtNetwork -lQtScript
INCLUDES= -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtScript -I/usr/include/qt4

MOCS=$(patsubst %.h, %.moc.cpp, $(shell grep -l Q_OBJECT *.h))
SOURCES=Main.cpp Memory.cpp Hook.cpp ReceivingMessageEvent.cpp Handler.cpp Message.cpp EncryptedMessage.cpp DecryptedMessage.cpp Packet.cpp ReadOnlyPacket.cpp ReadWritePacket.cpp Encryption.cpp ScriptHandler.cpp ScriptEngine.cpp $(MOCS)
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=hook.so

all: $(MOCS) $(SOURCES) $(LIBRARY)

clean:
	-rm hook.so *.o *.moc.cpp

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.moc.cpp: %.h
	$(MOC) $(INCLUDES) $< -o $@

$(LIBRARY): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -shared -o $@
