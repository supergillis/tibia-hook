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

#include "BattleListModel.h"

#include <BattleList.h>
#include <PluginLoader.h>

BattleListModel::BattleListModel(PluginManagerInterface* plugins, QObject* parent):
    QAbstractTableModel(parent) {
    battleList_ = PluginLoader<BattleListPluginInterface>(plugins)("battlelist");
}

int BattleListModel::columnCount(const QModelIndex&) const {
    return 2;
}

int BattleListModel::rowCount(const QModelIndex&) const {
    return battleList_->entries().size();
}

QVariant BattleListModel::data(const QModelIndex& index, int role) const {
    if(role == Qt::DisplayRole) {
        BattleListEntryInterface* entry = battleList_->entries()[index.row()];
        if(index.column() == 0) {
            return QVariant(entry->id());
        }
        return QVariant(entry->rawName());
    }
    return QVariant();
}

QModelIndex BattleListModel::index(int row, int column, const QModelIndex&) const {
    return createIndex(row, column);
}

QModelIndex BattleListModel::parent(const QModelIndex&) const {
    return QModelIndex();
}
