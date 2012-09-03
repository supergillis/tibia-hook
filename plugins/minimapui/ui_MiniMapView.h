/********************************************************************************
** Form generated from reading UI file 'MiniMapView.ui'
**
** Created: Mon Sep 3 19:16:53 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINIMAPVIEW_H
#define UI_MINIMAPVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MiniMapView
{
public:

    void setupUi(QWidget *MiniMapView)
    {
        if (MiniMapView->objectName().isEmpty())
            MiniMapView->setObjectName(QString::fromUtf8("MiniMapView"));
        MiniMapView->resize(400, 300);

        retranslateUi(MiniMapView);

        QMetaObject::connectSlotsByName(MiniMapView);
    } // setupUi

    void retranslateUi(QWidget *MiniMapView)
    {
        MiniMapView->setWindowTitle(QApplication::translate("MiniMapView", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MiniMapView: public Ui_MiniMapView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINIMAPVIEW_H
