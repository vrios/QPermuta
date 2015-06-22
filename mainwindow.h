#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "getrowcoldialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    getrowcoldialog *diag = new getrowcoldialog(this);
   // QTableWidget *table = new QTableWidget(1,1,this);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int nCols;
    int nRows;

public slots:
    void setCol(int );
    void setRow(int );

private slots:


    void on_actionNova_An_lise_triggered();
    void on_actionExit_triggered();

    void on_pushButtonAnalisar_clicked();

private:
    Ui::MainWindow *ui;

    void setTable();
    void printTable();
    void GenerateFiles();
};

#endif // MAINWINDOW_H
