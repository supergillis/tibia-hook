/********************************************************************************
** Form generated from reading UI file 'BattleListWidget.ui'
**
** Created: Mon Sep 3 21:09:50 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BATTLELISTWIDGET_H
#define UI_BATTLELISTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>
#include "BattleListView.h"

QT_BEGIN_NAMESPACE

class Ui_BattleListWidget
{
public:
    QHBoxLayout *horizontalLayout;
    BattleListView *tableView;

    void setupUi(QWidget *BattleListWidget)
    {
        if (BattleListWidget->objectName().isEmpty())
            BattleListWidget->setObjectName(QString::fromUtf8("BattleListWidget"));
        BattleListWidget->resize(400, 300);
        horizontalLayout = new QHBoxLayout(BattleListWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableView = new BattleListView(BattleListWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        horizontalLayout->addWidget(tableView);


        retranslateUi(BattleListWidget);

        QMetaObject::connectSlotsByName(BattleListWidget);
    } // setupUi

    void retranslateUi(QWidget *BattleListWidget)
    {
        BattleListWidget->setWindowTitle(QApplication::translate("BattleListWidget", "Battle List", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BattleListWidget: public Ui_BattleListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BATTLELISTWIDGET_H
