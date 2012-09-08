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

#include "WalkerPlugin.h"

#include <PositionTrackerPluginInterface.h>

#include <stdexcept>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.walker, WalkerPlugin)
#endif

WalkerPlugin::WalkerPlugin():
    walker_(NULL) {
}

void WalkerPlugin::install(HookInterface* hook, SettingsInterface* settings) throw(std::exception) {
    QObject* plugin = hook->plugins()->findPluginByName("positiontracker");
    if(plugin == NULL) {
        throw std::runtime_error("The 'positiontracker' plugin must be loaded before loading the 'minimapui' plugin!");
    }

    PositionTrackerPluginInterface* positionPlugin = qobject_cast<PositionTrackerPluginInterface*>(plugin);
    if(positionPlugin == NULL) {
        throw std::runtime_error("The 'positiontracker' plugin could not be loaded!");
    }

    walker_ = new Walker(hook->sender(), positionPlugin);
}

void WalkerPlugin::uninstall() {
    // Clean up objects
    delete walker_;
    walker_ = NULL;
}

void WalkerPlugin::walk(const QList<Position>& path) {
    walker_->walk(path);
}
