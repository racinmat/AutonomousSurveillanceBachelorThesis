/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *start;
    QSpinBox *countUav;
    QComboBox *map;
    QPushButton *stop;
    QGraphicsView *graphicsView;
    QTextBrowser *text;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1514, 1024);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        start = new QPushButton(centralWidget);
        start->setObjectName(QStringLiteral("start"));
        start->setGeometry(QRect(10, 60, 75, 23));
        countUav = new QSpinBox(centralWidget);
        countUav->setObjectName(QStringLiteral("countUav"));
        countUav->setGeometry(QRect(20, 10, 42, 22));
        map = new QComboBox(centralWidget);
        map->setObjectName(QStringLiteral("map"));
        map->setGeometry(QRect(90, 10, 66, 22));
        map->setDuplicatesEnabled(false);
        stop = new QPushButton(centralWidget);
        stop->setObjectName(QStringLiteral("stop"));
        stop->setGeometry(QRect(100, 60, 75, 23));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(500, 10, 1000, 1000));
        text = new QTextBrowser(centralWidget);
        text->setObjectName(QStringLiteral("text"));
        text->setGeometry(QRect(10, 90, 461, 901));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        start->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
        map->clear();
        map->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Map 0", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 1", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 2", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 3", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 4", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 5", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 6", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 7", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 8", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 9", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 10", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Map 11", Q_NULLPTR)
        );
        stop->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
