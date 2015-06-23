#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "getrowcoldialog.h"
#include <QString>
#include <iostream>
#include <vector>
#include <fstream>
#include<QTextStream>
#include <Qfile>
#include <QFileDialog>
using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(diag, SIGNAL(RowChanged(int )), this, SLOT(setRow(int)));
    connect(anDiag,SIGNAL(AnalysisChosen(QAbstractButton * )),this, SLOT(setAnalysis(QAbstractButton *)));
    connect(anDiag,SIGNAL(nPermutationsChosen(int )),this, SLOT(setNPermutations(int)));
    connect(anDiag,SIGNAL(StatisticChosen(int)),this, SLOT(setStatistics(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCol(int col )
{
    this->nCols=col;
}

void MainWindow::setRow(int row)
{
    this->nRows=row;
}

void MainWindow::setAnalysis(QAbstractButton *button)
{
    this->analysis= button->text();
}
void MainWindow::setNPermutations(int N)
{
    this->nPerm=N;
}

void MainWindow::setStatistics(QString stat)
{
    //inacabado
    switch (stat){
    case 1:
    this->statistics=aov();
        break;
    case 2:
        {this->statistics=aov();}
        break;

}

void MainWindow::setTable(){

    ui->myTable->clear();
    ui->myTable->setColumnCount(this->nCols);
    ui->myTable->setRowCount(this->nRows);
    // ui->myTable->horizontalHeader()->;

    QString na="NA";
    for (int i = 0; i < ui->myTable->rowCount(); ++i) {
        for (int j = 0; j < ui->myTable->columnCount(); ++j) {
            //QTableWidgetItem *cell = ui->myTable->item(i, j);
            // cell->setText(na);
            QTableWidgetItem* NA = new QTableWidgetItem();
            NA->setText("NA");
            ui->myTable->setItem(i,j,NA);
        }
    }
    ui->myTable->show();


}

void MainWindow::GenerateFiles(){
    //write data file
    QFile file("data.txt");
    file.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&file);

    if (file.exists()){// taken from http://www.qtcentre.org/threads/7602-Reading-writing-QTableWidget
        //QStringList headers;
        for(int i = 0; i < ui->myTable->model()->columnCount(); i++)http://stackoverflow.com/questions/22944787/how-to-get-list-of-headers-of-a-qtableview
        {
            // headers.append(myTableView->model()->headerData(i, Qt::Horizontal).toString();
            out<<ui->myTable->model()->headerData(i, Qt::Horizontal).toString();
            if (i!=ui->myTable->model()->columnCount()-1){out<<",";}
        }
        out<<"\n";
        for (int i = 0; i < ui->myTable->rowCount(); ++i) {
            if (i > 0)
                out << '\n';
            for (int j = 0; j < ui->myTable->columnCount(); ++j) {
                if (j > 0)
                    out << ',';
                QTableWidgetItem *cell = ui->myTable->item(i, j);
                out << cell->text().replace(",","."); //convert comma to dot
            }
        }}
    file.close();

    //write analysis.r file
    QFile file2("analysis.R");
    file2.open(QFile::WriteOnly | QFile::Text);
    QTextStream out2(&file2);

    if (file2.exists()){
        out2<<"library Rsampling;"<<"\n";
        out2<<"read.table(\"data.txt\", header=TRUE, sep=\",\")";
        out2<<"source("<<this->statistics<<")\n";
        out2<<"resampling(\""<<this->analysis<<",nPerm="<<this->nPerm<<",stat="<<this->statistics<<")";
    }
    file2.close();

}


void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_pushButtonEscolherAnalise_clicked()
{
    anDiag->setModal(true);
    anDiag->exec();
}

void MainWindow::on_actionAbrir_Arquivo_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Escolha seu arquivo de dados"),
                QDir::currentPath(),
                "All files (*.*);;Text File(*.txt);;csv file(*.csv)"
                );
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString str = in.readAll();
    QStringList rows = str.split('\n');

    int numRows = rows.count();
    int numColumns = rows.first().count(',') + 1;
    ui->myTable->setRowCount(numRows);
    ui->myTable->setColumnCount(numColumns);
     QStringList headers = rows[0].split(',');
     ui->myTable->setHorizontalHeaderLabels(headers);
    for (int i = 1; i < numRows; ++i) {
        QStringList columns = rows[i].split(',');
        for (int j = 0; j < numColumns; ++j) {
            QTableWidgetItem* cell = new QTableWidgetItem();
            cell->setText(columns[j]);
            ui->myTable->setItem(i,j,cell);

        }
    }
}

void MainWindow::on_actionNovosDados_triggered()
{
    diag->setModal(true);
    diag->exec();
    this->setTable();
}

void MainWindow::on_pushButtonSalvarArquivo_clicked()
{
    this->GenerateFiles();
}

void MainWindow::on_pushButtonSalvarAnalisar_clicked()
{
    anDiag->setModal(true);
    anDiag->exec();
    this->GenerateFiles();
    QString pathToR = QFileDialog::getExistingDirectory(
                this,
                tr("Escolha em que diretório está o R para análise"),
                QDir::currentPath(),
                QFileDialog::ReadOnly
                | QFileDialog::ShowDirsOnly
                );
    //system("R CMD BATCH analysis.R");
    string z = "cd " + pathToR.toStdString() + "&& Rscript analysis.R && pause";
    const char* c = z.c_str();
    system(c );
    //system("Rscript analysis.R");
}
