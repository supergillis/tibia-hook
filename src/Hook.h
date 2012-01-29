#ifndef HOOK_H
#define HOOK_H

#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QQueue>

#include <unistd.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

/* Including  <X11/Xlib.h> gives an error with Qt. Undefining Bool fixes that. */
#undef Bool

#include "EncryptedMessage.h"
#include "DecryptedMessage.h"
#include "ReadOnlyPacket.h"
#include "ReadWritePacket.h"

class Handler;
class Hook: public QCoreApplication {
	Q_OBJECT

public:
	static Hook* getInstance() {
		return _instance;
	}

	static Hook* initialize() {
		if(_instance == NULL) {
			_instance = new Hook();
		}
		return _instance;
	}

	int socket() const;
	void setSocket(int);

	const Display* display() const;
	void setDisplay(Display*);

	Window window() const;
	void setWindow(Window);

	bool hasClientMessages() const;

	ssize_t receiveFromClient(const quint8*, ssize_t);
	ssize_t receiveFromServer(quint8*, ssize_t);

	void sendToClient(const quint8*, ssize_t);
	void sendToClient(const EncryptedMessage*);
	void sendToClient(const DecryptedMessage*);

	ssize_t sendToServer(const quint8*, ssize_t);
	ssize_t sendToServer(const EncryptedMessage*);
	ssize_t sendToServer(const DecryptedMessage*);

	void sendKeyPress(int);

private:
	Hook();
	Hook(const Hook&);

	Hook& operator=(const Hook&);

	static Hook* _instance;

	int _socket;
	Display* _display;
	Window _window;

	Handler* _handler;
	bool _loggedIn;
	bool _pendingLogin;
	quint8 _protocol;
	QQueue<QByteArray> _queue;
};

#endif
