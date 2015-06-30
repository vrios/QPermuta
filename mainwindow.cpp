#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "getrowcoldialog.h"
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <string>
using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(diag, SIGNAL(ColChanged(int )), this, SLOT(setCol(int)));
    connect(diag, SIGNAL(RowChanged(int )), this, SLOT(setRow(int)));
    connect(anDiag,SIGNAL(AnalysisChosen(QAbstractButton * )),this, SLOT(setAnalysis(QAbstractButton *)));
    connect(anDiag,SIGNAL(nPermutationsChosen(int )),this, SLOT(setNPermutations(int)));
    connect(anDiag,SIGNAL(StatisticChosen(QString)),this, SLOT(setStatistics(QString)));
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
this->statistics=stat;
}

void MainWindow::setTable(){

    ui->myTable->clear();
    ui->myTable->setColumnCount(this->nCols);
    ui->myTable->setRowCount(this->nRows);
    QString na="NA";
    for (int i = 0; i < ui->myTable->rowCount(); ++i) {
        for (int j = 0; j < ui->myTable->columnCount(); ++j) {
            QTableWidgetItem* NA = new QTableWidgetItem();
            NA->setText("NA");
            ui->myTable->setItem(i,j,NA);
        }
    }
    ui->myTable->show();


}

void MainWindow::GenerateFiles(){
    //write data file
    QFile datafile("data.txt");
    datafile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&datafile);

    if (datafile.exists()){// taken from http://www.qtcentre.org/threads/7602-Reading-writing-QTableWidget
        //QStringList headers;
        datafile.resize(0);
        for(int i = 0; i < ui->myTable->model()->columnCount()+1; i++)//http://stackoverflow.com/questions/22944787/how-to-get-list-of-headers-of-a-qtableview
        {
            //if (i < ui->myTable->model()->columnCount()){out << "xx";}
            out<<ui->myTable->model()->headerData(i, Qt::Horizontal).toString();
            if (i<ui->myTable->model()->columnCount()-1){out<<",";}
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
    datafile.close();

    //write analysis.r file
    QFile Rfile("analysis.R");
    Rfile.open(QFile::WriteOnly | QFile::Text| QFile::Truncate);
    QTextStream out2(&Rfile);

    if (Rfile.exists()){
        out2<<"library (Rsampling);"<<"\n";
        out2<<"dataframe = read.table(\"data.txt\", header=TRUE, sep=\",\");"<<"\n";
        //out2<<"source("<<this->statistics<<")\n";
        out2<<"Rsampling(type=\""<<this->analysis<<"\", dataframe, statistics=\""<<this->statistics<<"\",ntrials=\""<<this->nPerm<<")";
    }
    Rfile.close();

#ifdef Q_OS_WIN
    //write batch file to run R in windows
    QFile BatchFile("Rbatch.bat");
    BatchFile.open(QFile::WriteOnly | QFile::Text| QFile::Truncate);
    QTextStream out3(&BatchFile);
    if (BatchFile.exists()){
        BatchFile.resize(0);
        out3<<" @echo off"<<"\n";
        out3<<" echo Rbatch Started."<<"\n";
        out3<<" call Rpathset.bat"<<"\n"; ///https://code.google.com/p/batchfiles/
        out3<<" R CMD BATCH analysis.R"<<"\n";
        out3<<" echo Rbatch Ended."<<"\n";
        out3<<"pause";
    }
    BatchFile.close();
#endif

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

#ifdef Q_OS_WIN
    QProcess p;
    p.startDetached("cmd.exe", QStringList() << "/c" << "Rbatch.bat");
    if (p.waitForStarted())
    {
        p.waitForFinished();
        qDebug() << p.readAllStandardOutput();
    }
    else
        qDebug() << "Failed to start";
#endif

#ifdef Q_OS_UNIX
    QProcess p;
    p.startDetached("R CMD BATCH analysis.R");
    if (p.waitForStarted())
    {
        p.waitForFinished();
        qDebug() << p.readAllStandardOutput();
    }
    else
        qDebug() << "Failed to start";
#endif

}

void MainWindow::on_actionInstalar_RSampling_triggered()
{
    this->Install();
}
void MainWindow::RunCommand(QString Command)
{
#ifdef Q_OS_WIN
    QProcess p;
    string z = Command.toStdString();
    p.startDetached("cmd.exe", QStringList() << "/c" << z.c_str() );
    if (p.waitForStarted())
    {
        p.waitForFinished();
        qDebug() << p.readAllStandardOutput();
    }
    else
        qDebug() << p.readAllStandardOutput();
#endif

#ifdef Q_OS_UNIX
    QProcess p;
    p.startDetached("R CMD BATCH"<< Command.toStdString());
    if (p.waitForStarted())
    {
        p.waitForFinished();
        qDebug() << p.readAllStandardOutput();
    }
    else
        qDebug() << "Failed to start";
#endif
}

void MainWindow::on_pushButtonInstalar_clicked()
{
    this->Install();
}
void MainWindow::Install(){

    // QMessageBox msgBox;
    // msgBox.setWindowTitle("Instalar Rsampling");
    //msgBox.setText("Deseja instalar o Rsampling? (requer instalar o pacote devtools e suas dependencias no R)");
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Instalar Rsampling", "Deseja instalar o Rsampling? (requer instalar o pacote devtools e suas dependencias no R)",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QFile Rfile("install.R");
        Rfile.open(QFile::WriteOnly | QFile::Text| QFile::Truncate);
        QTextStream out2(&Rfile);
        if (Rfile.exists()){
            Rfile.resize(0);
            out2<<"   r <- getOption(\"repos\")"<<"\n";
            out2<<"   r[\"CRAN\"] <- \"http://cran.us.r-project.org\""<<"\n";
            out2<<"   options(repos = r)"<<"\n";
            out2<<"   rm(r)"<<"\n";
            out2<<"    if(!is.element(\"devtools\", installed.packages()[,1])) install.packages(\"devtools\", dep=TRUE)"<<"\n";
            out2<<"    library(\"devtools\")"<<"\n";
            out2<<"   install_github(repo = \'lageIBUSP/Rsampling\')"<<"\n";

        }
        Rfile.close();

#ifdef Q_OS_WIN
        //write batch file to run R in windows
        QFile BatchFile("installBatch.bat");
        BatchFile.open(QFile::WriteOnly | QFile::Text| QFile::Truncate);
        QTextStream out3(&BatchFile);
        if (BatchFile.exists()){
            BatchFile.resize(0);
            out3<<" @echo off"<<"\n";
            out3<<" echo install Started."<<"\n";
            out3<<" call Rpathset.bat"<<"\n"; ///https://code.google.com/p/batchfiles/
            out3<<" R CMD BATCH install.R"<<"\n";
            out3<<" echo Rbatch Ended."<<"\n";
            out3<<"pause";
        }
        BatchFile.close();
        QProcess p;
        p.startDetached("cmd.exe", QStringList() << "/c" << "installBatch.bat");
        if (p.waitForStarted())
        {
            p.waitForFinished();
            qDebug() << p.readAllStandardOutput();
        }
        else
        {qDebug() << "Failed to start";}
        //Rfile.remove();
        //BatchFile.remove();

#endif

#ifdef Q_OS_UNIX
        QProcess p;
        p.startDetached("R CMD BATCH install.R");
        if (p.waitForStarted())
        {
            p.waitForFinished();
            qDebug() << p.readAllStandardOutput();
        }
        else
        {qDebug() << "Failed to start";}
#endif

    }
    else
    {//reply..close();
    }


    //    msgBox.setWindowTitle("Intalar Rsampling");
    //    msgBox.setText("Deseja instalar o Rsampling? (requer o pacote devtools instalado no R)");
    //    msgBox.setStandardButtons(QMessageBox::Yes);
    //    msgBox.setDefaultButton(QMessageBox::No);
    //    if(msgBox.exec() == QMessageBox::Yes){
    //        ///http://stackoverflow.com/questions/19596359/install-package-library-if-not-installed
    //        QString installer;
    //        installer ="usePackage <- function(p) {if (!is.element(p, installed.packages()[,1]))\\ninstall.packages(p, dep = TRUE)\\nrequire(p, character.only = TRUE)\\n}\\nusePackage(devtools)\\nlibrary(devtools)\\ninstall_github(repo = 'lageIBUSP/Rsampling');\n";
    //        this->RunCommand(installer);
    //    }
    //    else {
    //        close();
    //    }

}

void MainWindow::on_pushButtonInserirDados_clicked()
{
    diag->setModal(true);
    diag->exec();
    this->setTable();
}

void MainWindow::on_actionSobre_o_Qpermuta_triggered()
{


     QMessageBox::about(this,QString::fromLatin1("Sobre o Qpermuta 1.0 beta"),QString::fromUtf8("QPermuta \u00E9 uma interface gr\u00E1fica para o pacote Rsampling, dispon\u00EDvel em https://github.com/lageIBUSP/Rsampling \n\n Q Permuta foi escrito para facilitar a entrada e an\u00E1lise de dados para os que n\u00E3o tem familiaridade com a interface padr\u00E3o do R \n Qpermuta escrito por Vitor Rios \n Rsampling escrito por Paulo Prado \n vers\u00E3o mais recente dispon\u00EDvel em https://github.com/vrios/QPermuta/"));
}
