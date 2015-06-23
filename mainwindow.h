#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "getrowcoldialog.h"
#include "getanalysesdialog.h"
#include <QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    getrowcoldialog *diag = new getrowcoldialog(this);
    getAnalysesDialog *anDiag = new getAnalysesDialog(this);
   // QTableWidget *table = new QTableWidget(1,1,this);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int nCols;
    int nRows;
    QString analysis;
    QString statistics;
    int nPerm;

public slots:
    void setCol(int );
    void setRow(int );
    void setAnalysis(QAbstractButton * );
    void setNPermutations(int );
    void setStatistics(int);

private slots:


    void on_actionNovosDados_triggered();
    void on_actionExit_triggered();
    void on_pushButtonEscolherAnalise_clicked();
    void on_actionAbrir_Arquivo_triggered();
    void on_pushButtonSalvarArquivo_clicked();
    void on_pushButtonSalvarAnalisar_clicked();

private:
    Ui::MainWindow *ui;

    void setTable();
    void printTable();
    void GenerateFiles();
};

#endif // MAINWINDOW_H
