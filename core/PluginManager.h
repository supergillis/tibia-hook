#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "Settings.h"

#include <PluginManagerInterface.h>

#include <QDir>
#include <QList>
#include <QObject>
#include <QPair>

class Hook;
class PluginInfo: public QObject {
public:
    typedef QList<PluginInfo*> List;
    typedef QMutableListIterator<PluginInfo*> MutableListIterator;

    typedef QPair<QString, quint16> Dependency;
    typedef QList<Dependency> Dependencies;

    PluginInfo(const QString&, QObject* = 0);
    ~PluginInfo();

    const QString& libraryPath() const;
    const QString& name() const;
    const quint16 version() const;
    const PluginInfo::Dependencies& dependencies() const;

    SettingsInterface* settings();

private:
    QString libraryPath_;
    QString name_;
    quint16 version_;
    PluginInfo::Dependencies dependencies_;
    Settings* settings_;
};

class PluginManager: public QObject, public PluginManagerInterface {
public:
    typedef QMap<PluginInfo*, QObject*> PluginMap;
    typedef QPair<PluginInfo*, PluginInfo*> Dependency;
    typedef QList<Dependency> Dependencies;
    typedef QMutableListIterator<Dependency> MutableDependencyIterator;

    PluginManager(Hook*);
    ~PluginManager();

    void load(const QString&);
    void unload();

    QObject* findPluginByName(const QString& name);
    QObject* findPluginByName(const QString& name, quint16 version);

private:
    PluginInfo* loadDirectory(const QString& directory);

    Hook* hook_;
    QString directory_;

    PluginMap plugins_;
    PluginInfo::List pluginInfos_;
};

#endif
