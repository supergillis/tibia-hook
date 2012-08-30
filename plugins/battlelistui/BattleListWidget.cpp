#include "BattleListWidget.h"
#include "ui_BattleListWidget.h"

BattleListWidget::BattleListWidget(QWidget* parent):
    QWidget(parent), ui_(new Ui::BattleListWidget) {
    ui_->setupUi(this);
}

BattleListWidget::~BattleListWidget() {
    delete ui_;
}

BattleListTable* BattleListWidget::battleListTable() {
    return ui_->tableView;
}
