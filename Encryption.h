#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

#include <stdint.h>

#include <QByteArray>

namespace Encryption {
	class XTEA {
	public:
		static bool decrypt(uint8_t*, uint16_t, const uint32_t []);
		static bool decrypt(QByteArray&, const uint32_t []);

		static bool encrypt(uint8_t*, uint16_t, const uint32_t []);
		static bool encrypt(QByteArray&, const uint32_t []);
	};
	class Adler {
	public:
		static uint32_t checksum(const uint8_t*, uint16_t);
		static uint32_t checksum(const QByteArray&);
	};
};

#endif /* ENCRYPTION_H_ */
