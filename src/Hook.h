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
