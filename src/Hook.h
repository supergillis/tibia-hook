#ifndef HOOK_H
#define HOOK_H

#include <QApplication>
#include <QByteArray>
#include <QEvent>
#include <QMainWindow>

#include <HookInterface.h>

#include "Handler.h"

class Hook: public QApplication, public HookInterface {
	Q_OBJECT

public:
	Hook();

	Handler* handler();
	void setHandler(Handler*);

	bool event(QEvent*);

	void sendToClient(const QByteArray&);
	void sendToServer(const QByteArray&);

	void receiveFromClient(const QByteArray&);
	void receiveFromServer(const QByteArray&);

private:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	Handler* handler_;
};

#endif
