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

BattleListModel::BattleListModel(const BattleList* list, QObject* parent):
    QAbstractTableModel(parent), list_(list) {
}

int BattleListModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

int BattleListModel::rowCount(const QModelIndex& parent) const {
    return BATTLELIST_LENGTH;
}

QVariant BattleListModel::data(const QModelIndex& index, int role) const {
    if(role == Qt::DisplayRole) {
        BattleListEntry entry = list_->entries[index.row()];
        if(index.column() == 0) {
            return QVariant(entry.id);
        }
        return QVariant((char*) entry.name);
    }
    return QVariant();
}

QModelIndex BattleListModel::index(int row, int column, const QModelIndex& parent) const {
    return createIndex(row, column);
}

QModelIndex BattleListModel::parent(const QModelIndex& index) const {
    return QModelIndex();
}
