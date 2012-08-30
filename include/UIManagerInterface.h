#ifndef UIMANAGERINTERFACE_H
#define UIMANAGERINTERFACE_H

#include <QWidget>

class UIManagerInterface {
public:
    virtual ~UIManagerInterface() {}

    virtual void addTab(QWidget* tab, const QString& label) = 0;
    virtual void removeTab(QWidget* tab) = 0;
};

#endif
