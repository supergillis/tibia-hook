#ifndef HOOK_H
#define HOOK_H

#include <QCoreApplication>
#include <QQueue>

#include "Handler.h"

class Hook: public QCoreApplication {
	Q_OBJECT

public:
	Hook();

	Handler* handler();
	void setHandler(Handler*);

	void sendToClient(const Packet*);
	void sendToServer(const Packet*);

	void receiveFromClient(bool);
	void receiveFromServer();

private:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	Handler* handler_;
};

#endif
