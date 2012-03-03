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
#include "ScriptReceiver.h"
#include "ScriptPluginLoader.h"
#include "Sender.h"

#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QList>

#ifdef Q_WS_WIN
#define PLUGIN_NAME "plugin.dll"
#else
#define PLUGIN_NAME "plugin.so"
#endif

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
	Application* application = new Application();

	SenderInterface* sender = new Sender(DetourManager::instance());
	ScriptReceiver* receiver = new ScriptReceiver(sender);

	// Load plugins
	QList<QFileInfo> pluginsInfo = QDir("plugins").entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo& pluginInfo, pluginsInfo) {
		ScriptPluginLoader loader(QDir(pluginInfo.absoluteFilePath()).filePath(PLUGIN_NAME));
		ScriptPluginInterface* plugin = loader.instance();
		if (plugin) {
			receiver->install(plugin);
		}
	}

	// Load the main script
	receiver->reload();

	new Connector(receiver, sender);

	application->exec();
	return NULL;
}
