#include "UIManager.h"

UIManager::UIManager() {
    tabs_ = new QTabWidget(this);
    setCentralWidget(tabs_);
    setVisible(true);
    setWindowTitle("Tibia Hook");
}

void UIManager::addTab(QWidget* tab, const QString& label) {
    tabs_->addTab(tab, label);
}

void UIManager::removeTab(QWidget* tab) {
    int index = tabs_->indexOf(tab);
    if(index != -1) {
        tabs_->removeTab(index);
    }
}
