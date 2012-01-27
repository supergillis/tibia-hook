#ifndef MEMORY_H_
#define MEMORY_H_

#include <QtGlobal>
#include <QString>

class Memory {
public:
	static quint8 readU8(quint32);
	static quint16 readU16(quint32);
	static quint32 readU32(quint32);
	static QString readString(quint32);
};

#endif /* MEMORY_H_ */
