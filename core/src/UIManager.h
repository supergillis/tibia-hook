#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QMainWindow>
#include <QTabWidget>

#include <UIManagerInterface.h>

class UIManager: private QMainWindow, public UIManagerInterface {
public:
    UIManager(QWidget* parent = NULL);

    void addTab(QWidget* tab, const QString& label);
    void removeTab(QWidget* tab);

private:
    QTabWidget* tabs_;
};

#endif
