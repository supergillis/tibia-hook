#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

#include <stdint.h>

#include <QByteArray>

namespace Encryption {
	class XTEA {
	public:
		static QByteArray decrypt(QByteArray&, uint32_t []);
		static QByteArray encrypt(QByteArray&, uint32_t []);
		static uint32_t checksum(QByteArray&);
	};
};

#endif /* ENCRYPTION_H_ */
