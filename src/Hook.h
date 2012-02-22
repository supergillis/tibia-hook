#ifndef HOOK_H
#define HOOK_H

#include <QApplication>
#include <QMainWindow>

#include "Handler.h"

class Hook: public QApplication {
	Q_OBJECT

public:
	Hook();

	Handler* handler();
	void setHandler(Handler*);

	void sendToClient(const QByteArray&);
	void sendToServer(const QByteArray&);

	bool receiveFromClient(const QByteArray&);
	void receiveFromServer(const QByteArray&);

private:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	Handler* handler_;
};

#endif
