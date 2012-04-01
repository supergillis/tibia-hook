/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
	Connector(SenderInterface* sender, ReceiverInterface* receiver, QObject* parent = 0): QObject(parent), sender_(sender), receiver_(receiver) {
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
		receiver_->receiveFromServer(data);
	}

private:
	ReceiverInterface* receiver_;
	SenderInterface* sender_;
};

#endif
