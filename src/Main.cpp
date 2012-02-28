#include <pthread.h>

#include "Hook.h"
#include "ScriptHandler.h"
#include "ScriptPluginLoader.h"

#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QList>

void hook_constructor() __attribute__((constructor));
void* hook_thread(void*);

void hook_constructor() {
	pthread_t hook_id;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&hook_id, &attr, hook_thread, NULL);
}

void* hook_thread(void*) {
	Hook* hook = new Hook();
	ScriptHandler* handler = new ScriptHandler(hook);

	// Load plugins
	QList<QFileInfo> pluginsInfo = QDir("plugins").entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo& pluginInfo, pluginsInfo) {
		ScriptPluginLoader loader(QDir(pluginInfo.absoluteFilePath()).filePath("plugin.so"));
		ScriptPluginInterface* plugin = loader.instance();
		if (plugin) {
			handler->install(plugin);
		}
	}

	// Load the main script
	handler->reload();

	hook->setHandler(handler);
	hook->exec();
	return NULL;
}
