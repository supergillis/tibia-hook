#include "BattleListTableModel.h"

#include <QDebug>

BattleListTableModel::BattleListTableModel(const BattleList* list, QObject* parent):
    QAbstractTableModel(parent), list_(list) {
}

int BattleListTableModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

int BattleListTableModel::rowCount(const QModelIndex& parent) const {
    return BATTLELIST_LENGTH;
}

QVariant BattleListTableModel::data(const QModelIndex& index, int role) const {
    if(role == Qt::DisplayRole) {
        BattleListEntry entry = list_->entries[index.row()];
        if(index.column() == 0) {
            return QVariant(entry.id);
        }
        else {
            return QVariant((char*) entry.name);
        }
    }
    return QVariant();
}

QModelIndex BattleListTableModel::index(int row, int column, const QModelIndex& parent) const {
    return createIndex(row, column);
}

QModelIndex BattleListTableModel::parent(const QModelIndex& index) const {
    return QModelIndex();
}
