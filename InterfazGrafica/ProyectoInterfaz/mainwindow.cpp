#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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

void MainWindow::refreshFromBinaryPlayer(QTableWidget *tableWidget){
    std::vector<Player<long>> registros = playersSequentialFile.load();
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

void MainWindow::refreshFromBinaryTeams(QTableWidget *tableWidget){
    std::vector<Team<long>> registros = teamsSequentialFile.load();
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
    //refresh(ui->tableWidgetPlayers,"data/Players.csv");
    //refresh(ui->tableWidgetTeams,"data/Teams.csv");
    refreshFromBinaryPlayer(ui->tableWidgetPlayers);
    refreshFromBinaryTeams(ui->tableWidgetTeams);
    ui->consultaHashOut->clear();

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
    Team<long> temp(stol(results[4]),
                    results[5].c_str(),
                    stol(results[6]),
                    stol(results[7]),
                    stol(results[8]),
                    stol(results[9]),
                    stol(results[10]),
                    stol(results[11]),
                    stol(results[12]),
                    stol(results[13]),
                    stol(results[14]));
    teamsSequentialFile.insert(temp);
    this->on_recargar_clicked();
}

void MainWindow::addPlayers(std::vector<std::string> results)
{
    Player<long> temp(stol(results[4]),
            results[5].c_str(),
            results[6].c_str(),
            results[7].c_str(),
            stol(results[8]),
            stol(results[9]),
            stol(results[10]),
            stol(results[11]),
            stol(results[12]));
    playersSequentialFile.insert(temp);
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
    refreshFromBinaryPlayerHASH(ui->tableWidgetPlayers,"data/PlayersSequentialFile.bin");
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

        try {
            if(results[3]=="players"){
                generatePlayer(playersSequentialFile.search(stol(results[7])));
            }else if(results[3]=="teams"){
                generateTeams(teamsSequentialFile.search(stol(results[7])));
            } else {
                errorDeFormato.setText("Error en el nombre de la tabla.");
                errorDeFormato.exec();
            }
        }  catch (...) {
            errorDeFormato.setText("El ID a buscar no existe.");
            errorDeFormato.exec();
        }

    }else if(results[0]=="DELETE"){
        if(results[2]=="players"){
            playersSequentialFile.deleteRecord(stol(results[6]));
            errorDeFormato.setText( QString("El elemento con el ID ") + results[6].c_str() + QString(" se eliminó correctamente."));
            errorDeFormato.exec();
            this->on_recargar_clicked();
        }else if(results[2]=="teams"){
            teamsSequentialFile.deleteRecord(stol(results[6]));
            errorDeFormato.setText( QString("El elemento con el ID ") + results[6].c_str() + QString(" se eliminó correctamente."));
            errorDeFormato.exec();
            this->on_recargar_clicked();
        } else {
            errorDeFormato.setText("Error en el nombre de la tabla.");
            errorDeFormato.exec();
        }

    }else if(results[0]=="INSERT"){
        try {
            if(results[2]=="players"){
                addPlayers(results);
                errorDeFormato.setText(results[5].c_str() + QString(" se insertó correctamente"));
                errorDeFormato.exec();
            }else if(results[2]=="teams"){
                addTeams(results);
                errorDeFormato.setText(results[5].c_str() + QString(" se insertó correctamente"));
                errorDeFormato.exec();
            } else {
                errorDeFormato.setText("Error en el nombre de la tabla.");
                errorDeFormato.exec();
            }
        }  catch (...) {
            errorDeFormato.setText("Error en el insertado. No puedes insertar un elemento que ya existe.");
            errorDeFormato.exec();
        }

    } else {
        errorDeFormato.setText("Error en el método SELECT, DELETE o INSERT.");
        errorDeFormato.exec();
    }

}

void MainWindow::on_pushButtonHash_clicked()
{
    this->hash =  ExtendibleHash<RecordHash<long>>(1,2,"test.dat");
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
    string actual;
    for(auto &elemento : results){
        if(elemento == "INSERT" or actual == "INSERT"){
            if(elemento == "DELETE"){
                actual = "DELETE";
                continue;
            }
            actual = "INSERT";
            if(elemento != "INSERT"){
                hash.insertRecord(RecordHash<long>(stol(elemento.c_str())));
            }
        }else if(elemento == "DELETE" or actual == "DELETE"){
            if(elemento == "INSERT"){
                actual = "INSERT";
                continue;
            }
            actual = "DELETE";
            if(elemento != "DELETE"){
                hash.removeRecord(stol(elemento.c_str()));
            }
        }
    }

    hash.print();
    ui->consultaHashOut->setPlainText(QString::fromStdString(hash.printToString()));


    remove("000.dat");
    remove("1.dat");
    remove("0.dat");
    remove("10.dat");
    remove("010.dat");
    remove("100.dat");
    remove("110.dat");
    remove("test.dat");

}
