#include "Encryption.h"

const quint32* Encryption::XTEA::TIBIA_KEY = (quint32*) XTEA_START;

bool Encryption::XTEA::decrypt(QByteArray& encrypted, const quint32 key[]) {
	return decrypt((quint8*) encrypted.data(), encrypted.length(), key);
}

bool Encryption::XTEA::decrypt(quint8* buffer, quint16 length, const quint32 key[]) {
	if (length % 8 == 0) {
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

bool Encryption::XTEA::encrypt(QByteArray& decrypted, const quint32 key[]) {
	return encrypt((quint8*) decrypted.data(), decrypted.length(), key);
}

bool Encryption::XTEA::encrypt(quint8* buffer, quint16 length, const quint32 key[]) {
	if (length % 8 == 0) {
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

quint32 Encryption::Adler::checksum(const QByteArray& bytes) {
	return checksum((const quint8*) bytes.data(), bytes.length());
}

quint32 Encryption::Adler::checksum(const quint8* buffer, quint16 length) {
	quint32 a = 1, b = 0;
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
	return (b << 16) | a;
}
