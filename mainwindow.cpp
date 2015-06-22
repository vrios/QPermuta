#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "getrowcoldialog.h"
#include <QString>
#include <iostream>
#include <vector>
#include <fstream>
#include<QTextStream>
#include <Qfile>
using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::cout<<connect(diag, SIGNAL(ColChanged(int )), this, SLOT(setCol(int)));
    std::cout<<"  evil";
    connect(diag, SIGNAL(RowChanged(int )), this, SLOT(setRow(int)));
    ui->pushButtonAnalisar->hide();

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

void MainWindow::on_actionNova_An_lise_triggered()
{

    diag->setModal(true);
    diag->exec();


    this->setTable();
}

void MainWindow::setTable(){

    ui->myTable->clear();
    ui->myTable->setColumnCount(this->nCols);
    ui->myTable->setRowCount(this->nRows);
    // ui->myTable->horizontalHeader()->;
    ui->myTable->show();
    ui->pushButtonAnalisar->show();

}

void MainWindow::GenerateFiles(){
    //convert comma to dot


    //write
    QFile file("data.txt");
    file.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&file);

    if (file.exists()){
        out<<ui->myTable->horizontalHeader();
        for (int i = 0; i < ui->myTable->rowCount(); ++i) {
            if (i > 0)
                out << '\n';
            for (int j = 0; j < ui->myTable->columnCount(); ++j) {
                if (j > 0)
                    out << '\t';
                QTableWidgetItem *cell = ui->myTable->item(i, j);
                out << cell->text().replace(",",".");
            }
        }}
    file.close();

}

void MainWindow::printTable(){
    vector<QString>aux(ui->myTable->rowCount(),0);

    vector <vector<QString>> tempTable (ui->myTable->rowCount(),aux);
    tempTable.clear();
    for (int i; i=0; i< ui->myTable->rowCount())
    {
        for (int j; j=0; j<ui->myTable->columnCount()){
            tempTable[i][j] = ui->myTable->item(i,j)->text();
        }
    }

}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_pushButtonAnalisar_clicked()
{
    this->GenerateFiles();

}
