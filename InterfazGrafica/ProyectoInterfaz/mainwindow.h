#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>


#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>

//#include "Record.h"
#include "SequentialFile/SequentialFile.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void refresh(QTableWidget *tableWidget, std::string fileName);
    void refreshFromBinaryPlayer(QTableWidget *tableWidget, std::string fileName);
    void refreshFromBinaryTeams(QTableWidget *tableWidget, std::string fileName);

private slots:
    void on_recargar_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
