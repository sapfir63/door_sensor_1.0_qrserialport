/** @file Заголовочный файл формы с информацией о программе. */

#ifndef FRMABOUT_H
#define FRMABOUT_H

#include <QDialog>

namespace Ui {
    class frmAbout;
}

class frmAbout : public QDialog
{
    Q_OBJECT

public:
    explicit frmAbout(QWidget *parent = 0);
    ~frmAbout();

    void setSupportUrl(QString sUrl);


private slots:
    void on_btnClose_clicked();

private:
    Ui::frmAbout *ui;

};

#endif // FRMABOUT_H
