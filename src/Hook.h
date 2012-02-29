#ifndef HOOK_H
#define HOOK_H

#include <QByteArray>
#include <QCoreApplication>
#include <QEvent>
#include <QMainWindow>

#include <HookInterface.h>

#include "Handler.h"

class Hook: public QCoreApplication, public HookInterface {
	Q_OBJECT

	static int argc_;

public:
	Hook();

	Handler* handler();
	void setHandler(Handler*);

	void sendToClient(const QByteArray&);
	void sendToServer(const QByteArray&);

public slots:
	void receiveFromClient(const QByteArray&);
	void receiveFromServer(const QByteArray&);

private:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	Handler* handler_;
};

#endif
