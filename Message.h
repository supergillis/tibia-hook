#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>
#include <QByteArray>

class Message {
public:
	Message();
	Message(const uint8_t*, uint16_t);

	virtual bool isValid() const = 0;

	virtual uint16_t length() const = 0;
	virtual const uint8_t* data() const = 0;

	uint16_t rawLength() const;
	const uint8_t* rawData() const;

private:
	QByteArray _raw;
};

#endif /* MESSAGE_H_ */
