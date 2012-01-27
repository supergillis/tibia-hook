#include "Memory.h"

quint8 Memory::readU8(quint32 address) {
	quint8 value = *((quint8*) address);
	return value;
}

quint16 Memory::readU16(quint32 address) {
	quint16 value = *((quint16*) address);
	return value;
}

quint32 Memory::readU32(quint32 address) {
	quint32 value = *((quint32*) address);
	return value;
}

QString Memory::readString(quint32 address) {
	return QString((const char*) address);
}
