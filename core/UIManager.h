#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QMainWindow>
#include <QTabWidget>

#include <UIManagerInterface.h>

class UIManager: public UIManagerInterface, private QMainWindow
{
public:
    UIManager();

    void addTab(QWidget* tab, const QString& label);
    void removeTab(QWidget* tab);

private:
    QTabWidget* tabs_;
};

#endif
