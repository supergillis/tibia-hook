#include "encryption.h"

#include <QDebug>

bool Encryption::XTEA::decrypt(QByteArray& encrypted, uint32_t key[]) {
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
		return true;
	}
	return false;
}

bool Encryption::XTEA::encrypt(QByteArray& decrypted, uint32_t key[]) {
	int length = decrypted.length();
	if (length % 8 == 0) {
		uint8_t* buffer = (uint8_t*) decrypted.data();
		unsigned int position = 0;
		unsigned int buffer_32_length = length / 4;
		unsigned int* buffer_32 = (unsigned int*) buffer;
		while (position < buffer_32_length) {
			unsigned int v0 = buffer_32[position], v1 = buffer_32[position + 1];
			unsigned int delta = 0x61C88647;
			unsigned int sum = 0;
			for (int i = 0; i < 32; i++) {
				v0 += ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
				sum -= delta;
				v1 += ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[sum >> 11 & 3]);
			}
			buffer_32[position] = v0;
			buffer_32[position + 1] = v1;
			position = position + 2;
		}
		return true;
	}
	return false;
}

uint32_t Encryption::Adler::checksum(const QByteArray& data) {
	qDebug() << "adler1";
	const uint8_t* buffer = (const uint8_t*) data.constData();
	qDebug() << "adler2";
	uint16_t length = data.length();
	qDebug() << "adler3" << length;
	uint32_t a = 1, b = 0;
	while (length > 0) {
		size_t tlen = length > 5552 ? 5552 : length;
		length -= tlen;
		do {
			a += *buffer++;
			b += a;
		}
		while (--tlen);
		a %= 65521;
		b %= 65521;
	}
	qDebug() << "adler4";
	return (b << 16) | a;
}
