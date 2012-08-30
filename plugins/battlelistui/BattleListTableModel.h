#ifndef BATTLELISTTABLEMODEL_H
#define BATTLELISTTABLEMODEL_H

#include <QAbstractTableModel>

#include <BattleList.h>

class BattleListTableModel: public QAbstractTableModel {
    Q_OBJECT

public:
    BattleListTableModel(const BattleList*, QObject* = 0);

    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;

private:
    const BattleList* list_;
};

#endif
