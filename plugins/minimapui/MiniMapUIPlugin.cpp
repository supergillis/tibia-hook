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

#include "MiniMapUIPlugin.h"
#include "MiniMapModel.h"

#include <MiniMapPlugin.h>

#include <stdexcept>

#include <QtPlugin>
Q_EXPORT_PLUGIN2(minimapui, MiniMapUIPlugin)

const QString MiniMapUIPlugin::PLUGIN_NAME("minimapui");
const int MiniMapUIPlugin::PLUGIN_VERSION(1);

MiniMapUIPlugin::MiniMapUIPlugin(): view_(NULL) {
}

QString MiniMapUIPlugin::name() const {
    return PLUGIN_NAME;
}

int MiniMapUIPlugin::version() const {
    return PLUGIN_VERSION;
}

void MiniMapUIPlugin::install(HookInterface* hook) throw(std::exception) {
    QObject* plugin = hook->plugins()->findPluginByName("minimap");
    if(plugin == NULL) {
        throw std::runtime_error("The 'minimap' plugin must be loaded before loading the 'minimapui' plugin!");
    }

    MiniMapPluginInterface* miniMapPlugin = qobject_cast<MiniMapPluginInterface*>(plugin);
    if(miniMapPlugin == NULL) {
        throw std::runtime_error("The 'minimap' plugin could not be loaded!");
    }

    view_ = new MiniMapView();
    view_->setModel(new MiniMapModel(miniMapPlugin->miniMap()));
    ui_ = hook->ui();
    ui_->addTab(view_, "Map");
}

void MiniMapUIPlugin::uninstall() {
    if(view_ != NULL) {
        ui_->removeTab(view_);
        view_->deleteLater();
        view_ = NULL;
        ui_ = NULL;
    }
}
