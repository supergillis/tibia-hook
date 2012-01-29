#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

/* 9.44 addresses */
#define XTEA_START 0x85d8830
#define XTEA_LENGTH 0x04

#include <QByteArray>

namespace Encryption {
	class XTEA {
	public:
		static const quint32* TIBIA_KEY;

		static bool decrypt(quint8*, quint16, const quint32 []);
		static bool decrypt(QByteArray&, const quint32 []);

		static bool encrypt(quint8*, quint16, const quint32 []);
		static bool encrypt(QByteArray&, const quint32 []);
	};
	class Adler {
	public:
		static quint32 checksum(const quint8*, quint16);
		static quint32 checksum(const QByteArray&);
	};
};

#endif /* ENCRYPTION_H_ */
