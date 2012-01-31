#ifndef PACKET_H_
#define PACKET_H_

#include <QDebug>
#include <QObject>
#include <QString>

#define PACKET_END_OF_FILE "reached the end of the buffer"

class Packet: public QObject {
	Q_OBJECT
	Q_PROPERTY(quint16 length READ length)
	Q_PROPERTY(quint16 position WRITE setPosition READ position)

public:
	Packet();

	virtual quint16 length() const = 0;
	virtual const quint8* data() const = 0;

	quint16 position() const;
	void setPosition(quint16);

public slots:
	void skip(quint16 = 1);

	bool has(quint16) const;

	quint8 readU8();
	quint16 readU16();
	quint32 readU32();
	quint64 readU64();
	QString readString();

protected:
	quint16 position_;

};

#endif /* PACKET_H_ */
