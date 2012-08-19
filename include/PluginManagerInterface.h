#ifndef PLUGINMANAGERINTERFACE_H
#define PLUGINMANAGERINTERFACE_H

#include <QString>

class PluginInterface;
class PluginManagerInterface {
public:
    virtual PluginInterface* findPluginByName(const QString& name) = 0;
};

#endif // PLUGINMANAGERINTERFACE_H
