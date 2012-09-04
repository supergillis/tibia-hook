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

#include "Hook.h"
#include "Packet.h"
#include "PacketReader.h"
#include "PacketBuilder.h"
#include "UIManager.h"

#include <stdexcept>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QPluginLoader>

#define SETTING_PLUGINS_DIRECTORY "plugins_directory"

Hook::Hook(SettingsInterface* settings, SenderInterface* sender, QObject* parent) :
    QObject(parent), settings_(settings), sender_(sender) {
    ui_ = new UIManager();

    if(!settings_->contains(SETTING_PLUGINS_DIRECTORY)) {
        throw std::runtime_error("Could not load plugins directory!");
    }

    QString pluginsDir = settings->value(SETTING_PLUGINS_DIRECTORY).toString();

    // Load plugins
    QList<QFileInfo> pluginsInfo = QDir(pluginsDir).entryInfoList(QStringList(), QDir::Files);
    foreach(const QFileInfo& pluginInfo, pluginsInfo) {
        QPluginLoader loader(pluginInfo.absoluteFilePath());
        QObject* instance = loader.instance();

        // Check if it is a valid plugin
        PluginInterface* plugin = qobject_cast<PluginInterface*>(instance);
        if (plugin != 0) {
            qDebug() << "installing" << plugin->name();
            try {
                plugin->install(this);
                plugins_.append(instance);
            }
            catch(std::exception& exception) {
                QMessageBox message;
                message.setWindowTitle(QApplication::applicationName());
                message.setText("Could not load \"" + plugin->name() + "\" plugin!");
                message.setDetailedText(exception.what());
                message.setDefaultButton(QMessageBox::Ignore);
                message.exec();
            }
        }
    }
}

Hook::~Hook() {
    foreach(QObject* instance, plugins_) {
        PluginInterface* plugin = qobject_cast<PluginInterface*>(instance);
        if(plugin != 0) {
            qDebug() << "uninstalling" << plugin->name();
            plugin->uninstall();
        }
    }
}

bool Hook::receiveFromClient(const QByteArray& data) {
    Packet packet(data);
    PacketReader reader(&packet);
    quint8 type = reader.readU8();
    qDebug() << "client" << type << " length " << data.length();
    return true;
}

void Hook::receiveFromServer(const QByteArray& data) {
    Packet packet(data);
    PacketReader reader(&packet);
    quint8 type = reader.readU8();
    qDebug() << "server" << type << " length " << data.length();
}

QObject* Hook::findPluginByName(const QString& name) {
    foreach(QObject* instance, plugins_) {
        PluginInterface* plugin = qobject_cast<PluginInterface*>(instance);
        if(plugin != 0 && plugin->name().compare(name) == 0) {
            return instance;
        }
    }
    return NULL;
}

PacketBuilderInterface* Hook::buildPacket() const {
    return new PacketBuilder();
}

PacketBuilderInterface* Hook::buildPacket(const PacketInterface* packet) const {
    return new PacketBuilder(packet);
}

PacketBuilderInterface* Hook::buildPacket(const QByteArray& data) const {
    return new PacketBuilder(data);
}

PacketBuilderInterface* Hook::buildPacket(const quint8* data, quint16 length) const {
    return new PacketBuilder(data, length);
}
