#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

void MainWindow::on_recargar_clicked()
{
    refresh(ui->tableWidgetPlayers,"/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/Players.csv");
    refresh(ui->tableWidgetTeams,"/Users/christianledgard/Documents/2020-II/DBII/Proyecto/proyecto-DB2/InterfazGrafica/ProyectoInterfaz/Teams.csv");
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
            qDebug() << "players";
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
            qDebug() << "players";
        }else if(results[2]=="teams"){
            qDebug() << "teams";
        } else {
            errorDeFormato.setText("Error en el nombre de la tabla.");
            errorDeFormato.exec();
        }

    } else {
        errorDeFormato.setText("Error en el método SELECT, DELETE o INSERT.");
        errorDeFormato.exec();
    }

}
