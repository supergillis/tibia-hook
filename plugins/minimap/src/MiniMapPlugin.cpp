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

#include "MiniMapPlugin.h"
#include "MiniMapFloor.h"

#include <stdexcept>

#include <QVariantMap>
#include <QPainter>

#define SETTING_FOLDER "folder"

void MiniMapPlugin::install(HookInterface* hook, SettingsInterface* settings) throw(std::exception) {
    if(!settings->value(SETTING_FOLDER).isValid()) {
        throw std::runtime_error("Could not load minimap folder!");
    }

    miniMap_ = new MiniMap(settings->value(SETTING_FOLDER).toString());
}

void MiniMapPlugin::uninstall() {
}

MiniMapInterface* MiniMapPlugin::miniMap() {
    return miniMap_;
}

// Export plugin
Q_EXPORT_PLUGIN2(minimap, MiniMapPlugin)

MiniMap::MiniMap(const QString& directory): directory_(directory) {
}

MiniMapFloorInterface* MiniMap::createNewFloor(quint8 z) const {
    return new MiniMapFloor(directory_, z);
}
