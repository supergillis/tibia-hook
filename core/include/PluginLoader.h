#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <PluginManagerInterface.h>

#include <QtGlobal>

template<class PluginType>
class PluginLoader {
public:
    PluginLoader(PluginManagerInterface* manager):
        manager_(manager) {
    }

    PluginType* operator()(const QString& name) {
        QObject* plugin = manager_->findPluginByName(name);
        if (plugin == NULL) {
            return NULL;
        }
        return qobject_cast<PluginType*>(plugin);
    }

private:
    PluginManagerInterface* manager_;
};

#endif
