#ifndef PLUGINMANAGERINTERFACE_H
#define PLUGINMANAGERINTERFACE_H

#include <QString>

class PluginInterface;
class PluginManagerInterface {
public:
    virtual QObject* findPluginByName(const QString& name) = 0;
};

#endif
