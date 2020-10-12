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
#include "Hash/ExtendibleHashing.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    SequentialFile<Team<long>> teamsSequentialFile;
    SequentialFile<Player<long>> playersSequentialFile;
    ExtendibleHash<PlayerHash<long>> hash;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void refresh(QTableWidget *tableWidget, std::string fileName);
    void refreshFromBinaryPlayer(QTableWidget *tableWidget, std::string fileName);
    void refreshFromBinaryTeams(QTableWidget *tableWidget, std::string fileName);
    void generatePlayer(Player<long> it);
    void generateTeams(Team<long> it);
    void addTeams(std::vector<std::string> results);
    void addPlayers(std::vector<std::string> results);
    void addPlayersHASH(std::vector<std::string> results);
    void refreshFromBinaryPlayerHASH(QTableWidget *tableWidget, std::string fileName);


    void generatePlayer();

    void generateTeams();

    void addTeams();

private slots:
    void on_recargar_clicked();

    void on_pushButton_clicked();

    void on_pushButtonHash_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
