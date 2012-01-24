#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

#include <stdint.h>

#include <QByteArray>

namespace Encryption {
	class XTEA {
	public:
		static bool decrypt(QByteArray&, uint32_t []);
		static bool encrypt(QByteArray&, uint32_t []);
	};
	class Adler {
	public:
		static uint32_t checksum(const QByteArray&);
	};
};

#endif /* ENCRYPTION_H_ */
