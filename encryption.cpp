#include "encryption.h"

QByteArray Encryption::XTEA::decrypt(QByteArray& encrypted, uint32_t key[]) {
	int length = encrypted.length();
	if (length % 8 == 0) {
		uint8_t* buffer = (uint8_t*) encrypted.data();
		unsigned int position = 0;
		unsigned int buffer_32_length = length / 4;
		unsigned int* buffer_32 = (unsigned int*) buffer;
		while (position < buffer_32_length) {
			unsigned int v0 = buffer_32[position], v1 = buffer_32[position + 1];
			unsigned int delta = 0x61C88647;
			unsigned int sum = 0xC6EF3720;
			for (int i = 0; i < 32; i++) {
				v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[sum >> 11 & 3]);
				sum += delta;
				v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
			}
			buffer_32[position] = v0;
			buffer_32[position + 1] = v1;
			position = position + 2;
		}
		return QByteArray((char*) buffer, length);
	}
	return QByteArray();
}

QByteArray Encryption::XTEA::encrypt(QByteArray& decrypted, uint32_t key[]) {
	return QByteArray();
}

uint32_t Encryption::XTEA::checksum(QByteArray& data) {
	return 0;
}
