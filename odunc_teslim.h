#ifndef ODUNC_TESLIM_H
#define ODUNC_TESLIM_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class odunc_teslim;
}

class odunc_teslim : public QDialog
{
    Q_OBJECT

public:
    explicit odunc_teslim(QWidget *parent = nullptr);
    ~odunc_teslim();
private slots:
    void on_pushButton_odunc_ver_clicked();
    void listele();


private:
    Ui::odunc_teslim *ui;
    QSqlDatabase database;
    QSqlQuery *sorgu;
    QSqlQueryModel *model_odunc_alinan;
    QSqlQueryModel *model_odunc_teslim_edilen;
};

#endif // ODUNC_TESLIM_H
