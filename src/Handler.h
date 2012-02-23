#ifndef HANDLER_H_
#define HANDLER_H_

#include <QByteArray>

class Handler {
public:
	virtual bool receiveFromClient(const QByteArray&) = 0;
	virtual void receiveFromServer(const QByteArray&) = 0;
};

#endif /* HANDLER_H_ */
