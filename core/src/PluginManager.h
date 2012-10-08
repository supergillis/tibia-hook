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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QDir>
#include <QList>
#include <QObject>
#include <QPair>

#include <HookInterface.h>
#include <PluginManagerInterface.h>

#include "Settings.h"

class PluginInfo {
public:
    typedef QList<PluginInfo*> List;
    typedef QMutableListIterator<PluginInfo*> MutableListIterator;

    typedef QPair<QString, quint16> Dependency;
    typedef QList<Dependency> Dependencies;

    PluginInfo(const QString&);
    ~PluginInfo();

    SettingsInterface* settings();

    const PluginInfo::Dependencies& dependencies() const;
    const QString& libraryPath() const;
    const QString& name() const;
    quint16 version() const;

private:
    Settings* settings_;

    PluginInfo::Dependencies dependencies_;
    QString libraryPath_;
    QString name_;
    quint16 version_;
};

class PluginManager: public PluginManagerInterface {
public:
    typedef QMap<PluginInfo*, QObject*> PluginMap;
    typedef QPair<PluginInfo*, PluginInfo*> Dependency;
    typedef QList<Dependency> Dependencies;
    typedef QMutableListIterator<Dependency> MutableDependencyIterator;

    PluginManager(HookInterface*);
    ~PluginManager();

    void load(const QString&);
    void unload();

    QObject* findPluginByName(const QString& name);
    QObject* findPluginByName(const QString& name, quint16 version);

private:
    PluginInfo* loadDirectory(const QString& directory);

    HookInterface* hook_;
    QString directory_;

    PluginMap plugins_;
    PluginInfo::List pluginInfos_;
};

#endif
