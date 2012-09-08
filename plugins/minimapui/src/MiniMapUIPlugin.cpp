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

#include <MiniMapPluginInterface.h>
#include <PathFinderPluginInterface.h>
#include <PositionTrackerPluginInterface.h>
#include <WalkerPluginInterface.h>

#include <stdexcept>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.minimapui, MiniMapUIPlugin)
#endif

MiniMapUIPlugin::MiniMapUIPlugin():
    model_(NULL),
    view_(NULL) {
}

#include <QDebug>

void MiniMapUIPlugin::install(HookInterface* hook, SettingsInterface* settings) throw(std::exception) {
    QObject* plugin = hook->plugins()->findPluginByName("minimap");
    if(plugin == NULL) {
        throw std::runtime_error("The 'minimap' plugin must be loaded before loading the 'minimapui' plugin!");
    }

    MiniMapPluginInterface* miniMapPlugin = qobject_cast<MiniMapPluginInterface*>(plugin);
    if(miniMapPlugin == NULL) {
        throw std::runtime_error("The 'minimap' plugin could not be loaded!");
    }

    // Try to load the pathfinder plugin
    PathFinderPluginInterface* pathFinderPlugin = NULL;
    plugin = hook->plugins()->findPluginByName("pathfinder");
    if(plugin != NULL) {
        pathFinderPlugin = qobject_cast<PathFinderPluginInterface*>(plugin);
    }

    // Try to load the position plugin
    PositionTrackerPluginInterface* positionPlugin = NULL;
    plugin = hook->plugins()->findPluginByName("positiontracker");
    if(plugin != NULL) {
        positionPlugin = qobject_cast<PositionTrackerPluginInterface*>(plugin);
    }

    // Try to load the position plugin
    WalkerPluginInterface* walkerPlugin = NULL;
    plugin = hook->plugins()->findPluginByName("walker");
    if(plugin != NULL) {
        walkerPlugin = qobject_cast<WalkerPluginInterface*>(plugin);
    }

    model_ = new MiniMapModel(hook->sender(), miniMapPlugin, pathFinderPlugin, positionPlugin);
    view_ = new MiniMapView(positionPlugin, walkerPlugin);
    view_->setModel(model_);

    hook_ = hook;
    hook_->ui()->addTab(view_, "Map");
}

void MiniMapUIPlugin::uninstall() {
    // Clean up UI
    hook_->ui()->removeTab(view_);

    // Clean up objects
    delete view_;
    delete model_;
    view_ = NULL;
    model_ = NULL;
    hook_ = NULL;
}
