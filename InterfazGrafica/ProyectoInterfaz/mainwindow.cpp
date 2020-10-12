#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->teamsSequentialFile = SequentialFile<Team<long>>("/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/SequentialFile/data/Teams.bin", "/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/SequentialFile/data/TeamsSequentialFile.bin");
    this->playersSequentialFile = SequentialFile<Player<long>>("/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/SequentialFile/data/Players.bin", "/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/SequentialFile/data/PlayersSequentialFile.bin");
    this->hash = ExtendibleHash<PlayerHash<long>>("test.dat");
    ui->setupUi(this);
    this->on_recargar_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::refresh(QTableWidget *tableWidget, std::string fileName)
{
    QFile importedCSV(fileName.c_str());
    QString data;
    QStringList rowOfData;
    QStringList rowData;
    data.clear();
    rowOfData.clear();
    rowData.clear();
    tableWidget->setRowCount(0);

    if (importedCSV.open(QFile::ReadOnly))
    {
        data = importedCSV.readAll();
        rowOfData = data.split("\n");
        importedCSV.close();
    }

    tableWidget->setColumnCount(rowOfData.at(0).split(",").count());
    bool firstTime = true;

    for (int x = 0; x < rowOfData.size(); x++)
    {
        if(!firstTime)
            tableWidget->insertRow(tableWidget->rowCount());

        rowData = rowOfData.at(x).split(",");

        for (int y = 0; y < rowData.size(); y++)
        {
            if(firstTime){
                tableWidget->setHorizontalHeaderItem(y,new QTableWidgetItem(rowData[y]));
            } else {
                tableWidget->setItem(tableWidget->rowCount()-1,
                                         y,
                                         new QTableWidgetItem(rowData[y]));
            }
        }
        firstTime = false;
    }
}

void MainWindow::refreshFromBinaryPlayer(QTableWidget *tableWidget, std::string fileName){
    std::ifstream file(fileName, std::ios::binary);
    Player<long> registro;
    std::vector<Player<long>> registros;
    while (file.read((char *) &registro, sizeof(Player<long>))){
        registros.push_back(registro);
    }
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(11);
    tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("surname"));
    tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("team"));
    tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("position"));
    tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("minutes"));
    tableWidget->setHorizontalHeaderItem(5,new QTableWidgetItem("shots"));
    tableWidget->setHorizontalHeaderItem(6,new QTableWidgetItem("passes"));
    tableWidget->setHorizontalHeaderItem(7,new QTableWidgetItem("tackles"));
    tableWidget->setHorizontalHeaderItem(8,new QTableWidgetItem("saves"));
    tableWidget->setHorizontalHeaderItem(9,new QTableWidgetItem("next"));
    tableWidget->setHorizontalHeaderItem(10,new QTableWidgetItem("prev"));

    for (auto& it : registros) {
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setItem(tableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(it.ID)));
        tableWidget->setItem(tableWidget->rowCount()-1, 1, new QTableWidgetItem(it.surname));
        tableWidget->setItem(tableWidget->rowCount()-1, 2, new QTableWidgetItem(it.team));
        tableWidget->setItem(tableWidget->rowCount()-1, 3, new QTableWidgetItem(it.position));
        tableWidget->setItem(tableWidget->rowCount()-1, 4, new QTableWidgetItem(QString::number(it.minutes)));
        tableWidget->setItem(tableWidget->rowCount()-1, 5, new QTableWidgetItem(QString::number(it.shots)));
        tableWidget->setItem(tableWidget->rowCount()-1, 6, new QTableWidgetItem(QString::number(it.passes)));
        tableWidget->setItem(tableWidget->rowCount()-1, 7, new QTableWidgetItem(QString::number(it.tackles)));
        tableWidget->setItem(tableWidget->rowCount()-1, 8, new QTableWidgetItem(QString::number(it.saves)));
        tableWidget->setItem(tableWidget->rowCount()-1, 9, new QTableWidgetItem(QString::number(it.next)));
        tableWidget->setItem(tableWidget->rowCount()-1, 10, new QTableWidgetItem(QString::number(it.prev)));

    }


}

void MainWindow::refreshFromBinaryTeams(QTableWidget *tableWidget, std::string fileName){
    std::ifstream file(fileName, std::ios::binary);
    Team<long> registro;
    std::vector<Team<long>> registros;
    while (file.read((char *) &registro, sizeof(Team<long>))){
        registros.push_back(registro);
    }
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(13);
    tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("team"));
    tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("ranking"));
    tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("games"));
    tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("wins"));
    tableWidget->setHorizontalHeaderItem(5,new QTableWidgetItem("draws"));
    tableWidget->setHorizontalHeaderItem(6,new QTableWidgetItem("losses"));
    tableWidget->setHorizontalHeaderItem(7,new QTableWidgetItem("goalsFor"));
    tableWidget->setHorizontalHeaderItem(8,new QTableWidgetItem("goalsAgainst"));
    tableWidget->setHorizontalHeaderItem(9,new QTableWidgetItem("yellowCards"));
    tableWidget->setHorizontalHeaderItem(10,new QTableWidgetItem("redCards"));
    tableWidget->setHorizontalHeaderItem(11,new QTableWidgetItem("next"));
    tableWidget->setHorizontalHeaderItem(12,new QTableWidgetItem("prev"));

    for (auto& it : registros) {
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setItem(tableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(it.ID)));
        tableWidget->setItem(tableWidget->rowCount()-1, 1, new QTableWidgetItem(it.team));
        tableWidget->setItem(tableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(it.ranking)));
        tableWidget->setItem(tableWidget->rowCount()-1, 3, new QTableWidgetItem(QString::number(it.games)));
        tableWidget->setItem(tableWidget->rowCount()-1, 4, new QTableWidgetItem(QString::number(it.wins)));
        tableWidget->setItem(tableWidget->rowCount()-1, 5, new QTableWidgetItem(QString::number(it.draws)));
        tableWidget->setItem(tableWidget->rowCount()-1, 6, new QTableWidgetItem(QString::number(it.losses)));
        tableWidget->setItem(tableWidget->rowCount()-1, 7, new QTableWidgetItem(QString::number(it.goalsFor)));
        tableWidget->setItem(tableWidget->rowCount()-1, 8, new QTableWidgetItem(QString::number(it.goalsAgainst)));
        tableWidget->setItem(tableWidget->rowCount()-1, 9, new QTableWidgetItem(QString::number(it.yellowCards)));
        tableWidget->setItem(tableWidget->rowCount()-1, 10, new QTableWidgetItem(QString::number(it.redCards)));
        tableWidget->setItem(tableWidget->rowCount()-1, 11, new QTableWidgetItem(QString::number(it.next)));
        tableWidget->setItem(tableWidget->rowCount()-1, 12, new QTableWidgetItem(QString::number(it.prev)));
    }


}

void MainWindow::refreshFromBinaryPlayerHASH(QTableWidget *tableWidget, std::string fileName){
    std::ifstream file(fileName, std::ios::binary);
    Player<long> registro;
    std::vector<PlayerHash<long>> registros;
    while (file.read((char *) &registro, sizeof(Player<long>))){
        PlayerHash<long> temp;
        temp.ID = registro.ID;
        std::strcpy(temp.surname,registro.surname);
        std::strcpy(temp.team,registro.team);
        std::strcpy(temp.position,registro.position);
        temp.minutes = registro.minutes;
        temp.shots = registro.shots;
        temp.passes = registro.passes;
        temp.tackles = registro.tackles;
        temp.saves = registro.saves;
        temp.tackles = registro.tackles;
        temp.saves = registro.saves;
        registros.push_back(temp);
    }
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(11);
    tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("surname"));
    tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("team"));
    tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("position"));
    tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("minutes"));
    tableWidget->setHorizontalHeaderItem(5,new QTableWidgetItem("shots"));
    tableWidget->setHorizontalHeaderItem(6,new QTableWidgetItem("passes"));
    tableWidget->setHorizontalHeaderItem(7,new QTableWidgetItem("tackles"));
    tableWidget->setHorizontalHeaderItem(8,new QTableWidgetItem("saves"));
    tableWidget->setHorizontalHeaderItem(10,new QTableWidgetItem("prevDelete"));

    for (auto& it : registros) {
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setItem(tableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(it.ID)));
        tableWidget->setItem(tableWidget->rowCount()-1, 1, new QTableWidgetItem(it.surname));
        tableWidget->setItem(tableWidget->rowCount()-1, 2, new QTableWidgetItem(it.team));
        tableWidget->setItem(tableWidget->rowCount()-1, 3, new QTableWidgetItem(it.position));
        tableWidget->setItem(tableWidget->rowCount()-1, 4, new QTableWidgetItem(QString::number(it.minutes)));
        tableWidget->setItem(tableWidget->rowCount()-1, 5, new QTableWidgetItem(QString::number(it.shots)));
        tableWidget->setItem(tableWidget->rowCount()-1, 6, new QTableWidgetItem(QString::number(it.passes)));
        tableWidget->setItem(tableWidget->rowCount()-1, 7, new QTableWidgetItem(QString::number(it.tackles)));
        tableWidget->setItem(tableWidget->rowCount()-1, 8, new QTableWidgetItem(QString::number(it.saves)));
        tableWidget->setItem(tableWidget->rowCount()-1, 9, new QTableWidgetItem(QString::number(it.prevDelete)));
    }


}

void MainWindow::on_recargar_clicked()
{
    //refresh(ui->tableWidgetPlayers,"/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/Players.csv");
    //refresh(ui->tableWidgetTeams,"/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/Teams.csv");
    refreshFromBinaryPlayer(ui->tableWidgetPlayers,"/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/SequentialFile/data/PlayersSequentialFile.bin");
    refreshFromBinaryTeams(ui->tableWidgetTeams,"/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/SequentialFile/data/TeamsSequentialFile.bin");
}

void MainWindow::generatePlayer(Player<long> it)
{
    ui->tableWidgetPlayers->setRowCount(0);
    ui->tableWidgetPlayers->insertRow(ui->tableWidgetPlayers->rowCount());
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 0, new QTableWidgetItem(QString::number(it.ID)));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 1, new QTableWidgetItem(it.surname));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 2, new QTableWidgetItem(it.team));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 3, new QTableWidgetItem(it.position));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 4, new QTableWidgetItem(QString::number(it.minutes)));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 5, new QTableWidgetItem(QString::number(it.shots)));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 6, new QTableWidgetItem(QString::number(it.passes)));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 7, new QTableWidgetItem(QString::number(it.tackles)));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 8, new QTableWidgetItem(QString::number(it.saves)));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 9, new QTableWidgetItem(QString::number(it.next)));
    ui->tableWidgetPlayers->setItem(ui->tableWidgetPlayers->rowCount()-1, 10, new QTableWidgetItem(QString::number(it.prev)));
}

void MainWindow::generateTeams(Team<long> it)
{
    ui->tableWidgetTeams->setRowCount(0);
    ui->tableWidgetTeams->insertRow(ui->tableWidgetTeams->rowCount());
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 0, new QTableWidgetItem(QString::number(it.ID)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 1, new QTableWidgetItem(it.team));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 2, new QTableWidgetItem(QString::number(it.ranking)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 3, new QTableWidgetItem(QString::number(it.games)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 4, new QTableWidgetItem(QString::number(it.wins)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 5, new QTableWidgetItem(QString::number(it.draws)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 6, new QTableWidgetItem(QString::number(it.losses)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 7, new QTableWidgetItem(QString::number(it.goalsFor)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 8, new QTableWidgetItem(QString::number(it.goalsAgainst)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 9, new QTableWidgetItem(QString::number(it.yellowCards)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 10, new QTableWidgetItem(QString::number(it.redCards)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 11, new QTableWidgetItem(QString::number(it.next)));
    ui->tableWidgetTeams->setItem(ui->tableWidgetTeams->rowCount()-1, 12, new QTableWidgetItem(QString::number(it.prev)));
}

void MainWindow::addTeams(std::vector<std::string> results)
{
    Team<long> temp;
    temp.ID=stol(results[4]);
    std::strcpy(temp.team,results[5].c_str());
    temp.ranking = stol(results[6]);
    temp.games = stol(results[7]);
    temp.wins = stol(results[8]);
    temp.draws = stol(results[9]);
    temp.losses = stol(results[10]);
    temp.goalsFor = stol(results[11]);
    temp.goalsAgainst = stol(results[12]);
    temp.yellowCards = stol(results[13]);
    temp.redCards = stol(results[14]);
    teamsSequentialFile.insert(temp);
    this->on_recargar_clicked();
}

void MainWindow::addPlayers(std::vector<std::string> results)
{
    Team<long> temp;
    temp.ID=stol(results[4]);
    std::strcpy(temp.team,results[5].c_str());
    temp.ranking = stol(results[6]);
    temp.games = stol(results[7]);
    temp.wins = stol(results[8]);
    temp.draws = stol(results[9]);
    temp.losses = stol(results[10]);
    temp.goalsFor = stol(results[11]);
    temp.goalsAgainst = stol(results[12]);
    temp.yellowCards = stol(results[13]);
    temp.redCards = stol(results[14]);
    teamsSequentialFile.insert(temp);
    this->on_recargar_clicked();
}

void MainWindow::addPlayersHASH(std::vector<std::string> results)
{
    PlayerHash<long> temp;
    temp.ID=stol(results[4]);
    std::strcpy(temp.surname,results[5].c_str());
    std::strcpy(temp.team,results[6].c_str());
    std::strcpy(temp.position,results[7].c_str());
    temp.minutes = stol(results[8]);
    temp.shots = stol(results[9]);
    temp.passes = stol(results[10]);
    temp.tackles = stol(results[11]);
    temp.saves = stol(results[12]);
    hash.insertRecord(temp);
    refreshFromBinaryPlayerHASH(ui->tableWidgetPlayers,"/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/SequentialFile/data/PlayersSequentialFile.bin");
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox errorDeFormato;
    errorDeFormato.setText("Error en el input.");


    if(ui->consulta->toPlainText().isEmpty()){
        errorDeFormato.setText("Ingresar una consulta antes de continuar.");
        errorDeFormato.exec();
        return;
    }

    std::string s = ui->consulta->toPlainText().toUtf8().constData();

    std::istringstream iss(s);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());


    if(results[0]=="SELECT"){
        qDebug() << "SELECT";

        if(results[3]=="players"){
            generatePlayer(playersSequentialFile.searchInOrderedRecords(stol(results[7])));
        }else if(results[3]=="teams"){
            generateTeams(teamsSequentialFile.searchInOrderedRecords(stol(results[7])));
        } else {
            errorDeFormato.setText("Error en el nombre de la tabla.");
            errorDeFormato.exec();
        }


    }else if(results[0]=="DELETE"){
        qDebug() << "DELETE";

        if(results[2]=="players"){
            qDebug() << "players";
        }else if(results[2]=="teams"){
            qDebug() << "teams";
        } else {
            errorDeFormato.setText("Error en el nombre de la tabla.");
            errorDeFormato.exec();
        }

    }else if(results[0]=="INSERT"){
        if(results[2]=="players"){
            addPlayers(results);
        }else if(results[2]=="teams"){
            addTeams(results);
        } else {
            errorDeFormato.setText("Error en el nombre de la tabla.");
            errorDeFormato.exec();
        }

    } else {
        errorDeFormato.setText("Error en el método SELECT, DELETE o INSERT.");
        errorDeFormato.exec();
    }

}

void MainWindow::on_pushButtonHash_clicked()
{
    refreshFromBinaryPlayerHASH(ui->tableWidgetPlayers,"/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/SequentialFile/data/PlayersSequentialFile.bin");
    QMessageBox errorDeFormato;
    errorDeFormato.setText("Error en el input.");


    if(ui->consulta->toPlainText().isEmpty()){
        errorDeFormato.setText("Ingresar una consulta antes de continuar.");
        errorDeFormato.exec();
        return;
    }

    std::string s = ui->consulta->toPlainText().toUtf8().constData();

    std::istringstream iss(s);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());


    if(results[0]=="SELECT"){
        qDebug() << "SELECT";

        if(results[3]=="players"){

        }else if(results[3]=="teams"){
            qDebug() << "teams";
        } else {
            errorDeFormato.setText("Error en el nombre de la tabla.");
            errorDeFormato.exec();
        }


    }else if(results[0]=="DELETE"){
        qDebug() << "DELETE";

        if(results[2]=="players"){
            qDebug() << "players";
        }else if(results[2]=="teams"){
            qDebug() << "teams";
        } else {
            errorDeFormato.setText("Error en el nombre de la tabla.");
            errorDeFormato.exec();
        }

    }else if(results[0]=="INSERT"){
        if(results[2]=="players"){
            addPlayersHASH(results);
        }else if(results[2]=="teams"){
        } else {
            errorDeFormato.setText("Error en el nombre de la tabla.");
            errorDeFormato.exec();
        }

    } else {
        errorDeFormato.setText("Error en el método SELECT, DELETE o INSERT.");
        errorDeFormato.exec();
    }



}
