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

#include "Hook.h"
#include "DetourManager.h"

int Hook::argc_ = 0;

Hook::Hook() :
		QCoreApplication(Hook::argc_, NULL), handler_(NULL) {
	connect(DetourManager::instance(), SIGNAL(onClientMessage(QByteArray)), this, SLOT(receiveFromClient(const QByteArray&)));
	connect(DetourManager::instance(), SIGNAL(onServerMessage(QByteArray)), this, SLOT(receiveFromServer(const QByteArray&)));
}

Handler* Hook::handler() {
	return handler_;
}

void Hook::setHandler(Handler* handler) {
	handler_ = handler;
}

/**
  * This function runs in the injected thread.
  */
void Hook::sendToClient(const QByteArray& data) {
	DetourManager::instance()->clientQueue()->enqueue(data);
}

/**
  * This function runs in the injected thread.
  */
void Hook::sendToServer(const QByteArray& data) {
	DetourManager::instance()->serverQueue()->enqueue(data);
}

/**
  * This function runs in the injected thread.
  */
void Hook::receiveFromClient(const QByteArray& data) {
	if (handler_ && handler_->receiveFromClient(data)) {
		sendToServer(data);
	}
}

/**
  * This function runs in the injected thread.
  */
void Hook::receiveFromServer(const QByteArray& data) {
	if (handler_) {
		handler_->receiveFromServer(data);
	}
}
