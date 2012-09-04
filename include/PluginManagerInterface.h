#ifndef PLUGINMANAGERINTERFACE_H
#define PLUGINMANAGERINTERFACE_H

#include <QObject>
#include <QString>

class PluginManagerInterface {
public:
    virtual QObject* findPluginByName(const QString& name) = 0;
    virtual QObject* findPluginByName(const QString& name, quint16 version) = 0;
};

#endif
