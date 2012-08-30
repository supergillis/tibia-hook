#ifndef BATTLELISTVIEW_H
#define BATTLELISTVIEW_H

#include <QWidget>

#include "BattleListTable.h"

namespace Ui {
class BattleListWidget;
}

class BattleListWidget: public QWidget {
    Q_OBJECT
    
public:
    BattleListWidget(QWidget* parent = 0);
    ~BattleListWidget();
    
    BattleListTable* battleListTable();

private:
    Ui::BattleListWidget* ui_;
};

#endif
