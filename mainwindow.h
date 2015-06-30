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
    void setStatistics(QString);

private slots:


    void on_actionNovosDados_triggered();
    void on_actionExit_triggered();
    void on_pushButtonEscolherAnalise_clicked();
    void on_actionAbrir_Arquivo_triggered();
    void on_pushButtonSalvarArquivo_clicked();
    void on_pushButtonSalvarAnalisar_clicked();

    void on_actionInstalar_RSampling_triggered();

    void on_pushButtonInstalar_clicked();

    void on_pushButtonInserirDados_clicked();

    void on_actionSobre_o_Qpermuta_triggered();

private:
    Ui::MainWindow *ui;
    void RunCommand(QString Command);
    void setTable();
    void printTable();
    void GenerateFiles();
    void Install();
};

#endif // MAINWINDOW_H
