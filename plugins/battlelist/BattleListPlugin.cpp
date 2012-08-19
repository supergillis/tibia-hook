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

#include "BattleListPlugin.h"

#include <QtPlugin>
Q_EXPORT_PLUGIN2(battlelist, BattleListPlugin)

const QString BattleListPlugin::PLUGIN_NAME("battlelist");
const int BattleListPlugin::PLUGIN_VERSION(1);

#define SETTING_ADDRESS "address"

QString BattleListPlugin::name() const {
    return PLUGIN_NAME;
}

int BattleListPlugin::version() const {
    return PLUGIN_VERSION;
}

void BattleListPlugin::install(HookInterface* hook) throw(Exception) {
    QVariantMap settings = hook->settings()->value(PLUGIN_NAME).toMap();
    if(!settings.value(SETTING_ADDRESS).isValid())
        throw StringException("Could not load battlelist address!");

    list_ = (BattleList*) settings.value(SETTING_ADDRESS).toUInt();
}

void BattleListPlugin::uninstall() {
}

const BattleListEntry* BattleListPlugin::findById(const quint32 id) {
    for(int index = 0; index < BATTLELIST_LENGTH; ++index) {
        if(list_->entries[index].id == id) {
            return &list_->entries[index];
        }
    }
    return NULL;
}

const BattleListEntry* BattleListPlugin::findByName(const QString& name) {
    for(int index = 0; index < BATTLELIST_LENGTH; ++index) {
        if(name.compare((char*) list_->entries[index].name) == 0) {
            return &list_->entries[index];
        }
    }
    return NULL;
}
