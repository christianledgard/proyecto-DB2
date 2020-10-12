/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *consulta;
    QPushButton *pushButton;
    QPushButton *pushButtonHash;
    QPushButton *recargar;
    QTabWidget *tabWidget;
    QWidget *players;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidgetPlayers;
    QWidget *teams;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *tableWidgetTeams;
    QMenuBar *menubar;
    QMenu *menuDB_II;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(773, 546);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        verticalLayout_4->addLayout(gridLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        consulta = new QPlainTextEdit(centralwidget);
        consulta->setObjectName(QString::fromUtf8("consulta"));

        horizontalLayout->addWidget(consulta);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButtonHash = new QPushButton(centralwidget);
        pushButtonHash->setObjectName(QString::fromUtf8("pushButtonHash"));

        horizontalLayout->addWidget(pushButtonHash);

        recargar = new QPushButton(centralwidget);
        recargar->setObjectName(QString::fromUtf8("recargar"));
        recargar->setCheckable(false);

        horizontalLayout->addWidget(recargar);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(3, 1);

        verticalLayout_3->addLayout(horizontalLayout);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabBarAutoHide(false);
        players = new QWidget();
        players->setObjectName(QString::fromUtf8("players"));
        verticalLayout_2 = new QVBoxLayout(players);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidgetPlayers = new QTableWidget(players);
        tableWidgetPlayers->setObjectName(QString::fromUtf8("tableWidgetPlayers"));

        verticalLayout_2->addWidget(tableWidgetPlayers);

        tabWidget->addTab(players, QString());
        teams = new QWidget();
        teams->setObjectName(QString::fromUtf8("teams"));
        horizontalLayout_2 = new QHBoxLayout(teams);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tableWidgetTeams = new QTableWidget(teams);
        tableWidgetTeams->setObjectName(QString::fromUtf8("tableWidgetTeams"));

        horizontalLayout_2->addWidget(tableWidgetTeams);

        tabWidget->addTab(teams, QString());

        verticalLayout_3->addWidget(tabWidget);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 20);

        verticalLayout_4->addLayout(verticalLayout_3);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 773, 22));
        menuDB_II = new QMenu(menubar);
        menuDB_II->setObjectName(QString::fromUtf8("menuDB_II"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuDB_II->menuAction());

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Ejecutar Consulta", nullptr));
        pushButtonHash->setText(QCoreApplication::translate("MainWindow", "Ejecutar Consulta HASH", nullptr));
        recargar->setText(QCoreApplication::translate("MainWindow", "Recargar", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(players), QCoreApplication::translate("MainWindow", "Players", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(teams), QCoreApplication::translate("MainWindow", "Teams", nullptr));
        menuDB_II->setTitle(QCoreApplication::translate("MainWindow", "DB II", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
