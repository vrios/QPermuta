#include "getrowcoldialog.h"
#include "ui_getrowcoldialog.h"

getrowcoldialog::getrowcoldialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getrowcoldialog)
{
    ui->setupUi(this);
    col=1;row=1;
}

getrowcoldialog::~getrowcoldialog()
{
    delete ui;
}


void getrowcoldialog::on_buttonBox_accepted()
{
    this->col=ui->nColSpinBox->value();
    this->row=ui->nRowSpinBox->value();
    emit ColChanged(col);
    emit RowChanged(row);
    close();

}

void getrowcoldialog::on_buttonBox_rejected()
{
close();
}
