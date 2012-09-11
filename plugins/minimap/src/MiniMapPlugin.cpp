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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.minimap, MiniMapPlugin)
#endif

#define SETTING_FOLDER "folder"

void MiniMapPlugin::install(HookInterface*, SettingsInterface* settings) throw(std::exception) {
    if(!settings->value(SETTING_FOLDER).isValid()) {
        throw std::runtime_error("Could not load minimap folder!");
    }

    directory_ = settings->value(SETTING_FOLDER).toString();
}

void MiniMapPlugin::uninstall() {
    // Delete cached floors
    foreach (MiniMapFloorInterface* floor, cache_.values()) {
        delete floor;
    }
    cache_.clear();
    directory_.clear();
}

MiniMapFloorInterface* MiniMapPlugin::floor(quint8 floorIndex) {
    // Load image from cache or from the model
    MiniMapFloorInterface* floor;
    if (cache_.contains(floorIndex)) {
        floor = cache_.value(floorIndex);
    }
    else {
        floor = new MiniMapFloor(directory_, floorIndex);
        cache_.insert(floorIndex, floor);
    }
    return floor;
}
