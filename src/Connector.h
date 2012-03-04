#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QByteArray>
#include <QDebug>
#include <QObject>

#include <SenderInterface.h>
#include <ReceiverInterface.h>

#include "DetourManager.h"

class Connector: public QObject {
	Q_OBJECT

public:
	Connector(ReceiverInterface* receiver, SenderInterface* sender, QObject* parent = 0): QObject(parent), receiver_(receiver), sender_(sender) {
		QObject::connect(DetourManager::instance(), SIGNAL(onClientMessage(QByteArray)), this, SLOT(receiveFromClient(const QByteArray&)));
		QObject::connect(DetourManager::instance(), SIGNAL(onServerMessage(QByteArray)), this, SLOT(receiveFromServer(const QByteArray&)));
	}

private slots:
	void receiveFromClient(const QByteArray& data) {
		if(receiver_->receiveFromClient(data)) {
			sender_->sendToServer(data);
		}
	}

	void receiveFromServer(const QByteArray& data) {
		// Do nothing with result
		receiver_->receiveFromServer(data);
	}

private:
	ReceiverInterface* receiver_;
	SenderInterface* sender_;
};

#endif // CONNECTOR_H
