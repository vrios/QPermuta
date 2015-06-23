#ifndef GETROWCOLDIALOG_H
#define GETROWCOLDIALOG_H

#include <QDialog>
#include <vector>
namespace Ui {
class getrowcoldialog;
}

class getrowcoldialog : public QDialog
{
    Q_OBJECT

public:
    explicit getrowcoldialog(QWidget *parent = 0);
    ~getrowcoldialog();

private slots:

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void ColChanged(int );
    void RowChanged(int );

private:
    Ui::getrowcoldialog *ui;
    int col;
    int row;
};

#endif // GETROWCOLDIALOG_H
