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

#include "BattleListUIPlugin.h"
#include "BattleListModel.h"

#include <BattleListPluginInterface.h>
#include <UIManagerInterface.h>

#include <stdexcept>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.battlelistui, BattleListUIPlugin)
#endif

BattleListUIPlugin::BattleListUIPlugin(): widget_(NULL) {
}

void BattleListUIPlugin::install(HookInterface* hook, SettingsInterface*) throw(std::exception) {
    QObject* plugin = hook->plugins()->findPluginByName("battlelist");
    if(plugin == NULL) {
        throw std::runtime_error("The 'battlelist' plugin must be loaded before loading the 'battlelistui' plugin!");
    }

    BattleListPluginInterface* battleListPlugin = qobject_cast<BattleListPluginInterface*>(plugin);
    if(battleListPlugin == NULL) {
        throw std::runtime_error("The 'battlelist' plugin could not be loaded!");
    }

    widget_ = new BattleListWidget();
    widget_->battleListView()->setModel(new BattleListModel(hook->plugins()));
    ui_ = hook->ui();
    ui_->addTab(widget_, "Battle List");
}

void BattleListUIPlugin::uninstall() {
    if(widget_ != NULL) {
        ui_->removeTab(widget_);
        widget_->deleteLater();
        widget_ = NULL;
        ui_ = NULL;
    }
}
