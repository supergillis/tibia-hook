#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <QByteArray>

class Message {
public:
	Message();

	virtual bool isValid() const = 0;

	virtual quint16 length() const = 0;
	virtual const quint8* data() const = 0;

	quint16 rawLength() const;
	const quint8* rawData() const;

protected:
	virtual void initialize(const quint8*, quint16);

private:
	QByteArray _raw;
};

#endif /* MESSAGE_H_ */
