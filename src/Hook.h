#ifndef HOOK_H
#define HOOK_H

#include <QCoreApplication>
#include <QQueue>

#include "Handler.h"
#include "HookSocket.h"
#include "HookClient.h"
#include "EncryptedMessage.h"
#include "DecryptedMessage.h"

class Hook: public QCoreApplication {
	Q_OBJECT

public:
	Hook();

	Handler* handler();
	void setHandler(Handler*);

	HookSocket* socket();
	void setSocket(HookSocket*);

	HookClient* client();
	void setClient(HookClient*);

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
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	HookSocket* socket_;
	HookClient* client_;
	Handler* handler_;

	bool loggedIn_;
	bool pendingLogin_;
	QQueue<QByteArray> queue_;
};

#endif
