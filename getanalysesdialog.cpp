#include "getanalysesdialog.h"
#include "ui_getanalysesdialog.h"
#include <QButtonGroup>
getAnalysesDialog::getAnalysesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getAnalysesDialog)
{
    ui->setupUi(this);

     //QButtonGroup *analysis;
}

getAnalysesDialog::~getAnalysesDialog()
{
    delete ui;
}

void getAnalysesDialog::on_buttonBox_accepted()
{
    emit AnalysisChosen(ui->buttonGroup->checkedButton());
    emit nPermutationsChosen(ui->spinBox->value());
}

void getAnalysesDialog::on_textEdit_textChanged()
{
    emit StatisticChosen(ui->comboBox->currentIndex());
}
