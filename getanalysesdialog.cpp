#include "getanalysesdialog.h"
#include "ui_getanalysesdialog.h"
#include <QButtonGroup>
getAnalysesDialog::getAnalysesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getAnalysesDialog)
{
    ui->setupUi(this);
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
    emit StatisticChosen(ui->textEdit->toPlainText());
}

void getAnalysesDialog::on_buttonBox_rejected()
{
    close();
}

void getAnalysesDialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1!="personalizada")
    {
        ui->textEdit->setEnabled(false);
        ui->textEdit->clear();
        emit StatisticChosen(arg1);
    }
    else{
        ui->textEdit->setEnabled(true);
        ui->textEdit->setFocus();
    }
}
