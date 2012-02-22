#ifndef HOOK_H
#define HOOK_H

#include <QCoreApplication>

#include "Handler.h"

class Hook: public QCoreApplication {
	Q_OBJECT

public:
	Hook();

	Handler* handler();
	void setHandler(Handler*);

	void sendToClient(const QByteArray&);
	void sendToServer(const QByteArray&);

	bool receiveFromClient(const QByteArray&);
	bool receiveFromServer(const QByteArray&);

private:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	Handler* handler_;
};

#endif
