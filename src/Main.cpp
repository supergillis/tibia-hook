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

#include <pthread.h>

#include "Application.h"
#include "Connector.h"
#include "DetourManager.h"
#include "DetourSender.h"
#include "JsonConfig.h"
#include "ScriptHook.h"

#include <QMessageBox>

#include <Exception.h>

void hook_constructor() __attribute__((constructor));
void* hook_thread(void*);

/**
  * This function runs when the library is injected.
  */
void hook_constructor() {
	pthread_t hook_id;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&hook_id, &attr, hook_thread, NULL);
}

/**
  * This function runs when the thread is created. Qt runs in this thread.
  */
void* hook_thread(void*) {
	QApplication::setApplicationName("Tibia Hook");
	QApplication::setApplicationVersion("beta");

	Application* application = new Application();
	QFile configFile("config.js");
	try {
		if(!configFile.open(QFile::ReadOnly))
			throw Exception("Could not load config.js!");

		ConfigInterface* config = new JsonConfig(configFile.readAll());
		SenderInterface* sender = new DetourSender(DetourManager::instance());
		ReceiverInterface* receiver = new ScriptHook(config, sender);
		Connector* connector = new Connector(receiver, sender);
		application->exec();
	}
	catch(Exception& exception) {
		QMessageBox message;
		message.setWindowTitle(QApplication::applicationName());
		message.setText("Something terrible happened!");
		message.setDetailedText(exception.message());
		message.setDefaultButton(QMessageBox::Ignore);
		message.exec();
	}
	return NULL;
}
