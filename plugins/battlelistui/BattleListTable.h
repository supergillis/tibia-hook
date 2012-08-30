#ifndef BATTLELISTTABLE_H
#define BATTLELISTTABLE_H

#include <QTableView>

#include "BattleListTableModel.h"

class BattleListTable: public QTableView {
    Q_OBJECT

public:
    BattleListTable(QWidget* = 0);
    
};

#endif
