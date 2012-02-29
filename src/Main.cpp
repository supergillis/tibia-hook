#include <pthread.h>

#include "Hook.h"
#include "ScriptHandler.h"
#include "ScriptPluginLoader.h"

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
	Hook* hook = new Hook();
	ScriptHandler* handler = new ScriptHandler(hook);

	// Load plugins
	QList<QFileInfo> pluginsInfo = QDir("plugins").entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QFileInfo& pluginInfo, pluginsInfo) {
		ScriptPluginLoader loader(QDir(pluginInfo.absoluteFilePath()).filePath(PLUGIN_NAME));
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
