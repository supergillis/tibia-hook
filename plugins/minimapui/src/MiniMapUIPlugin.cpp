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
#include <UIManagerInterface.h>

#include <stdexcept>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.minimapui, MiniMapUIPlugin)
#endif

MiniMapUIPlugin::MiniMapUIPlugin():
    hook_(NULL),
    view_(NULL) {
}

void MiniMapUIPlugin::install(HookInterface* hook, SettingsInterface*) throw(std::exception) {
    view_ = new MiniMapView(this, hook->plugins());

    hook_ = hook;
    hook_->ui()->addTab(view_, "Map");
}

void MiniMapUIPlugin::uninstall() {
    hook_->ui()->removeTab(view_);

    delete view_;
}
