#ifndef ODUNC_ALMA_H
#define ODUNC_ALMA_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QModelIndex>


namespace Ui {
class odunc_alma;
}

class odunc_alma : public QDialog
{
    Q_OBJECT

public:
    explicit odunc_alma(QWidget *parent = nullptr);
    ~odunc_alma();

public slots:
    void odunc_al();
private slots:
    void listele();


private:
    Ui::odunc_alma *ui;
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQuery *sorgu;
};

#endif // ODUNC_ALMA_H
