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

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QPluginLoader>

#include <stdexcept>

#include <PluginInterface.h>

#include "PluginManager.h"
#include "JsonSettings.h"

#ifdef Q_OS_WIN
#define PLUGIN_WILDCARD "*.dll"
#else
#define PLUGIN_WILDCARD "*.so"
#endif

PluginInfo* findPluginInfoByName(PluginInfo::List& list, const QString& name, quint16 version) {
    for (PluginInfo::List::iterator it = list.begin(); it != list.end(); ++it) {
        PluginInfo* info = *it;
        if (info->name().compare(name) == 0 && info->version() >= version) {
            return info;
        }
    }
    return NULL;
}

PluginManager::PluginManager(HookInterface* hook):
    hook_(hook) {
}

PluginManager::~PluginManager() {
    unload();
}

void PluginManager::load(const QString& directory) {
    QFileInfoList plugins = QDir(directory).entryInfoList(QStringList(), QDir::Dirs);
    foreach (const QFileInfo& plugin, plugins) {
        PluginInfo* info = loadPluginInfo(plugin.absoluteFilePath());
        if (info != NULL) {
            pluginInfos_.append(info);
        }
    }

    // Generate dependency graph
    Dependencies dependencies;
    for (PluginInfo::List::iterator it = pluginInfos_.begin(); it != pluginInfos_.end(); ++it) {
        PluginInfo::Dependencies deps = (*it)->dependencies();
        for (PluginInfo::Dependencies::iterator depit = deps.begin(); depit != deps.end(); ++depit) {
            PluginInfo* dependency = findPluginInfoByName(pluginInfos_, depit->first, depit->second);
            if (dependency == NULL) {
                throw std::runtime_error(("Could not load '" + (*it)->name() + "': unmet dependency '" + depit->first + "'[" + QString::number(depit->second) + "]!").toStdString());
            }
            dependencies.append(Dependency(*it, dependency));
        }
    }

    PluginInfo::List independent(pluginInfos_);
    for (Dependencies::iterator it = dependencies.begin(); it != dependencies.end(); ++it) {
        independent.removeAll(it->first);
    }

    // Topological sort of plugins
    PluginInfo::List sorted;
    while (!independent.empty()) {
        PluginInfo* info = independent.takeFirst();
        sorted.append(info);

        MutableDependencyIterator it(dependencies);
        while (it.hasNext()) {
            // Copy constructor, else dependency.first won't work anymore
            Dependency dependency = it.next();
            if(dependency.second == info) {
                it.remove();
            }

            bool hasOtherDependencies = false;
            for (Dependencies::iterator it = dependencies.begin(); it != dependencies.end(); ++it) {
                if (it->first == dependency.first) {
                    hasOtherDependencies = true;
                    break;
                }
            }

            if (!hasOtherDependencies) {
                independent.append(dependency.first);
            }
        }
    }

    // Sorted state
    pluginInfos_ = sorted;

    // Load the plugins
    for (PluginInfo::List::iterator it = pluginInfos_.begin(); it != pluginInfos_.end(); ++it) {
        PluginInfo* info = *it;

        QPluginLoader loader(info->libraryPath());
        QObject* instance = loader.instance();

        if (instance == 0) {
            // TODO Error message
            continue;
        }

        // Check if it is a valid plugin
        PluginInterface* plugin = qobject_cast<PluginInterface*>(instance);
        if (plugin == 0) {
            // TODO Error message
            continue;
        }

        try {
            plugin->install(hook_, info->settings());
            plugins_.insert(info, instance);
        }
        catch(std::exception& exception) {
            QMessageBox message;
            message.setWindowTitle(QApplication::applicationName());
            message.setText("Could not load \"" + info->name() + "\" plugin!");
            message.setDetailedText(exception.what());
            message.setDefaultButton(QMessageBox::Ignore);
            message.exec();
        }
    }
}

void PluginManager::unload() {
    QListIterator<PluginInfo*> it(pluginInfos_);

    // Unload plugins in reverse order
    it.toBack();
    while (it.hasPrevious()) {
        PluginInfo* info = it.previous();
        PluginInterface* plugin = qobject_cast<PluginInterface*>(plugins_.value(info));

        qDebug() << "uninstalling" << info->name();
        plugin->uninstall();

        delete plugin;
        delete info;
    }
    plugins_.clear();
    pluginInfos_.clear();
}

QObject* PluginManager::findPluginByName(const QString& name) {
    for (PluginMap::iterator it = plugins_.begin(); it != plugins_.end(); ++it) {
        PluginInfo* info = it.key();
        if(info->name().compare(name) == 0) {
            return it.value();
        }
    }
    return NULL;
}

QObject* PluginManager::findPluginByName(const QString& name, quint16 version) {
    for (PluginMap::iterator it = plugins_.begin(); it != plugins_.end(); ++it) {
        PluginInfo* info = it.key();
        if(info->name().compare(name) == 0 && info->version() >= version) {
            return it.value();
        }
    }
    return NULL;
}

PluginInfo* PluginManager::loadPluginInfo(const QString& directory) {
    try {
        return new PluginInfo(directory);
    }
    catch(std::runtime_error& error) {
        qWarning() << error.what();
    }
    return NULL;
}

PluginInfo::PluginInfo(const QString& directory) {
    QDir dir(directory);
    QStringList candidates = dir.entryList(QStringList() << PLUGIN_WILDCARD, QDir::Files | QDir::NoDotAndDotDot);
    if (candidates.length() == 0) {
        throw std::runtime_error(("Could not load '" + directory + "': could not find shared library!").toStdString());
    }
    libraryPath_ = dir.absoluteFilePath(candidates.first());

    // Load metadata
    QString metaPath = dir.absoluteFilePath("meta.js");
    QFile metaFile(metaPath);
    if (!metaFile.exists()) {
        throw std::runtime_error(("Could not load '" + metaPath + "'!").toStdString());
    }
    if (!metaFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error(("Could not open '" + metaPath + "'!").toStdString());
    }

    JsonSettings meta;
    if (!meta.parse(metaFile.readAll())) {
        throw std::runtime_error(("Could not parse '" + metaPath + "'!").toStdString());
    }
    if (!meta.contains("name")) {
        throw std::runtime_error(("Could not plugin name in '" + metaPath + "'!").toStdString());
    }
    if (!meta.contains("version")) {
        throw std::runtime_error(("Could not version name in '" + metaPath + "'!").toStdString());
    }

    name_ = meta.value("name").toString();
    version_ = meta.value("version").toUInt();

    QVariantList deps = meta.value("dependencies").toList();
    for (QVariantList::iterator it = deps.begin(); it != deps.end(); ++it) {
        QVariantList dep = it->toList();

        QString name = dep.at(0).toString();
        quint16 version = dep.at(1).toUInt();
        dependencies_.append(PluginInfo::Dependency(name, version));
    }

    // Load config
    QString configPath = dir.absoluteFilePath("config.js");
    QFile configFile(configPath);
    if (!configFile.exists()) {
        throw std::runtime_error(("Could not load '" + configPath + "'!").toStdString());
    }
    if (!configFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error(("Could not open '" + configPath + "'!").toStdString());
    }
    if (!settings_.parse(configFile.readAll())) {
        throw std::runtime_error(("Could not parse '" + configPath + "'!").toStdString());
    }
}

SettingsInterface* PluginInfo::settings() {
    return &settings_;
}

const PluginInfo::Dependencies& PluginInfo::dependencies() const {
    return dependencies_;
}

const QString& PluginInfo::libraryPath() const {
    return libraryPath_;
}

const QString& PluginInfo::name() const {
    return name_;
}

quint16 PluginInfo::version() const {
    return version_;
}
