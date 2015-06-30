#ifndef GETANALYSESDIALOG_H
#define GETANALYSESDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QString>
namespace Ui {
class getAnalysesDialog;
}

class getAnalysesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getAnalysesDialog(QWidget *parent = 0);
    ~getAnalysesDialog();

private slots:
    void on_buttonBox_accepted();
    void on_textEdit_textChanged();

    void on_buttonBox_rejected();

    void on_comboBox_currentIndexChanged(const QString &arg1);

signals:
    void nPermutationsChosen(int);
    void AnalysisChosen(QAbstractButton * );
    void StatisticChosen(QString);


private:
    Ui::getAnalysesDialog *ui;
};

#endif // GETANALYSESDIALOG_H
