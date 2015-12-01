/********************************************************************************
** Form generated from reading UI file 'hci.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HCI_H
#define UI_HCI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HCIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *HCIClass)
    {
        if (HCIClass->objectName().isEmpty())
            HCIClass->setObjectName(QStringLiteral("HCIClass"));
        HCIClass->resize(600, 400);
        menuBar = new QMenuBar(HCIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        HCIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(HCIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        HCIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(HCIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        HCIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(HCIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        HCIClass->setStatusBar(statusBar);

        retranslateUi(HCIClass);

        QMetaObject::connectSlotsByName(HCIClass);
    } // setupUi

    void retranslateUi(QMainWindow *HCIClass)
    {
        HCIClass->setWindowTitle(QApplication::translate("HCIClass", "HCI", 0));
    } // retranslateUi

};

namespace Ui {
    class HCIClass: public Ui_HCIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HCI_H
