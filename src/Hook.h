#ifndef HOOK_H
#define HOOK_H

#include <QApplication>
#include <QMainWindow>
#include <QEvent>

#include "Handler.h"

class Hook: public QApplication {
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
