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
#include "BattleListEntry.h"

#include <SettingsInterface.h>

#include <QVariantMap>
#include <QDebug>

#include <stdexcept>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.battlelist, BattleListPlugin)
#endif

#define SETTING_ADDRESS "address"

#define BATTLELIST_SIZE 250
#define BATTLELIST_ENTRY_SIZE 176

void BattleListPlugin::install(HookInterface* hook, SettingsInterface* settings) throw(std::exception) {
    if(!settings->value(SETTING_ADDRESS).isValid()) {
        throw std::runtime_error("Could not load battlelist address!");
    }

    MemoryLocation base = hook->memory()->mapAddress(0x948008);
    for (int offset = 0; offset < BATTLELIST_SIZE * BATTLELIST_ENTRY_SIZE; offset += BATTLELIST_ENTRY_SIZE) {
        entries_.append(new BattleListEntry(hook->memory(), base + offset));
    }
}

void BattleListPlugin::uninstall() {
    foreach (BattleListEntryInterface* entry, entries_) {
        delete entry;
    }
    entries_.clear();
}

QList<BattleListEntryInterface*> BattleListPlugin::entries() const {
    return entries_;
}

BattleListEntryInterface* BattleListPlugin::findById(const quint32 id) const {
    foreach (BattleListEntryInterface* entry, entries_) {
        if (entry->id() == id) {
            return entry;
        }
    }
    return NULL;
}

BattleListEntryInterface* BattleListPlugin::findByName(const QString& name) const {
    foreach (BattleListEntryInterface* entry, entries_) {
        if (name.compare(entry->rawName()) == 0) {
            return entry;
        }
    }
    return NULL;
}
